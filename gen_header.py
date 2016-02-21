#!/usr/bin/python
#
# gen_header.py - generate portable header
#
# Copyright (C) 2013 Matthew R. Wette
# mwette@alumni.caltech.edu
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307, USA.
#
# The GNU General Public License is contained in the file COPYING.
#
# v130608a

import sys, os, re, difflib
import pdb

from gen_utils import replace_in_code, move_if_changed, write_w_fill

defcombos = [
    ('and', "__APPLE__", "__i386__"),
    ('and', "__APPLE__", "__x86_64__"),
    ('or', "__MINGW32__", "__CYGWIN32__", ('and', "_WIN32", "_M_IX86")),
    ('and', "__linux__", "__i386__"),
    ('and', "__linux__", "__x86_64__"),
    ('and', "__linux__", "__powerpc__", ('not', "__powerpc64__")),
    ('and', "__linux__", "__powerpc__", "__powerpc64__"),
    ('and', "__linux__", "__arm__"),
    ('and', "__linux__", "__x390__", "__s390x__"),
    ('and', "__linux__", "__mips__"),
    ]

rx1 = re.compile(r' *(VG_USERREQ_\w+).*')
rx2 = re.compile(r'#define (\w+)(.*)')

def slurp(filename):
    """
    Read header file and return list of enums and #defines for user codes.
    This will raise exception if file has no enums or defs
    """
    # Skip to enum, then read enum-guts.
    enums = []
    f0 = open(filename, 'r')
    l0 = f0.readline()
    while l0:
        if l0.startswith("   enum {"): break
        l0 = f0.readline()
    l0 = f0.readline()
    while l0:
        m = rx1.match(l0)
        if not m: break
        enums.append(m.group(1).strip())
        l0 = f0.readline()
    # Now process defs.
    defs = []
    while l0:
        if not l0.startswith("#define"):
            l0 = f0.readline()
            continue
        defn = l0
        while defn.endswith("\\\n"):
            defn = defn.rstrip(" \t\\\n")
            defn += " " + f0.readline().lstrip()
        if defn.find("VALGRIND_DO_CLIENT_REQUEST") >= 0:
            defs.append(defn)
        l0 = f0.readline()
    f0.close()
    return [enums,defs]

def get_enum_values(filename, enumnames):
    """
    For enums that exist int the provided header file return a dict that
    gives values for each enum.
    """
    f1 = open("_z1.c", 'w')
    f1.write("#include <stdio.h>\n")
    f1.write("#include \"%s\"\n" % (filename,))
    f1.write("int main() {\n")
    for e in enumnames:
        f1.write("  printf(\"enumd['%s'] = \\\"0x%%x\\\"\\n\", %s);\n" % (e,e))
    f1.write("}\n")
    f1.close()
    # run code and collect
    os.system("gcc -I../include -o _z1 _z1.c")
    f2 = os.popen("./_z1", 'r')
    enumd = {}
    l = f2.readline()
    while l:
        exec(l)
        l = f2.readline()
    f2.close()
    os.system("rm _z1 _z1.c")
    return enumd

rx3 = re.compile(r"#define\s+([A-Z0-9_]+)\s*(\(.*)")
rx4 = re.compile(r"#define\s+([A-Za-z0-9_]+)(\([A-Za-z0-9_, ]*\))\s*(.*)$")

def massage_defs(defs, enumd):
    """
    Given an array of defs, replace lval names and expand enum vals.
    """
    newdefs = []
    enuml = enumd.keys()
    for d0 in defs:
        m = rx4.match(d0)
        if not m:
            raise Exception, "can't handle " + d0
        if m.group(1).startswith("XPROF_"):
            lval = "#define " \
                + re.sub("XPROF_", "XPROF_", m.group(1)) + m.group(2)
        elif m.group(1).startswith("XP_"):
            lval = "#define " \
                + re.sub("XP_", "XP_", m.group(1)) + m.group(2)
        else:
            lval = "#define " + m.group(1) + m.group(2)
        rval = m.group(3)
        for e in enuml:
            if rval.find(e) >= 0:
                rval = rval.replace(e, enumd[e])
        newdefs.append(lval + " " + rval)
    return newdefs

def qr_mindefs(rule, defs=[]):
    """
    Generate min list of defs to satisfy rule.
    Example: ('and', "_x_", "_y_") -> ["_x_", "_y_"] 
    """
    def x_leaf(arg):
        return [arg]
    def x_and(args):
        res = []
        for a in args:
            res.extend(qr_mindefs(a))
        return res
    def x_or(args):
        return qr_mindefs(args[1])
    def x_not(args):
        return []
    if isinstance(rule, tuple):
        if rule[0] == 'and':
            return x_and(rule[1:])
        elif rule[0] == 'or':
            return x_or(rule[1:])
        elif rule[0] == 'not':
            return x_not(rule[1:])
    else:
        return x_leaf(rule)

def qr_expand(rule):
    """
    Expand qualifier to cpp code.  
    Example: ('and', "_x_", "_y_") -> (defined(_x_) && defined(_y_))
    """
    def x_leaf(arg):
        return "defined(" + arg + ")"
    def x_and(args):
        res = qr_expand(args[0])
        for a in args[1:]:
            res = res + " && " + qr_expand(a)
        return "(" + res + ")"
    def x_or(args):
        res = qr_expand(args[0])
        for a in args[1:]:
            res = res + " || " + qr_expand(a)
        return "(" + res + ")"
    def x_not(args):
        return "!" + qr_expand(args[0])
    if isinstance(rule, tuple):
        if rule[0] == 'and':
            return x_and(rule[1:])
        elif rule[0] == 'or':
            return x_or(rule[1:])
        elif rule[0] == 'not':
            return x_not(rule[1:])
    else:
        return x_leaf(rule)

def collect_qdefs(rexpr, defs):
    """
    Given all defrules return list of symbols used.
    """
    if isinstance(rexpr, tuple):
        for e in rexpr[1:]:
            collect_qdefs(e, defs)
    else:
        if not rexpr in defs:
            defs.append(rexpr)

def expand_defs(defs, qc, aq):
    """
    defs = #define we want to expand
    qc = qualifier combo (needs expand by qr_expand).
    aq = all qualifiers
    """
    f1 = open("_z1.c", 'w')
    for q in aq:
        f1.write("#undef " + q + "\n")
    for q in qr_mindefs(qc):
        f1.write("#define " + q + "\n")
    f1.write("#include \"valgrind.h\"\n")
    #
    f1.write('=== BEG ===\n')
    for d in defs:
        f1.write(re.sub("#define", "Edefine", d) + "\n")
    f1.write('=== END ===\n')
    f1.close()
    #
    cc_cmd = "cc"
    os.system(cc_cmd + " -E -I../include _z1.c >_z1.E")
    #
    f2 = open("_z1.E", 'r')
    xdefs = []
    l0 = f2.readline()
    while l0:
        if l0.startswith("=== BEG ==="):
           l0 = f2.readline()
           break
        l0 = f2.readline()
    while l0:
        if l0.startswith('=== END ==='):
            break
        if l0.startswith('Edefine'):
            # clean up ...
            l = l0
            l = re.sub('Edefine ', '#define ', l)
            #l = re.sub('_zzq', '', l)
            #l = re.sub('_qzz', '', l)
            xdefs.append(l)
        l0 = f2.readline()
    f2.close()
    os.system("rm -f _z1.c _z1.E")
    return xdefs

def genxhdr1(stuff, f1):
    """
    called by fill_in_xxx
    """
    qcl, xdefd = stuff
    nqc = len(qcl)
    defl = []
    el = ""
    for ix in range(nqc):
        qcstr = qr_expand(qcl[ix])[1:-1]
        xdefs = xdefd[ix]
        f1.write("#" + el + "if")
        col = 4
        parts = qcstr.split()
        for p in parts:
            col = write_w_fill(f1, col, " " + p, leader = "\t", cc = " \\")
        f1.write("\n\n")
        el = "el"
        for xdef in xdefs:
            m = rx4.match(xdef)
            if not m: raise Exception, "NO MATCH"
            lval = "#define " + m.group(1) + m.group(2)
            if ix == 0: defl.append(lval)
            f1.write(lval + "\t\\\n")
            lines = chopup(m.group(3), 60)
            n = len(lines)
            for i in range(n):
                f1.write("\t")
                f1.write(lines[i])
                if i < n -1: f1.write(" \\")
                f1.write("\n")
            f1.write("\n")
    f1.write("#else\n")
    for d in defl:
        #f1.write(d + " /* */\n")
        f1.write(d + " 0\n")
    f1.write('#endif\n')

def chopup(line, mx = 60):
    """
    Chop up into lines of max length mx (default 60).
    Deals with double-quotes.
    """
    lines = []
    ln = len(line)
    st = 0
    nd = st
    while nd < ln:
        if st + mx > ln:
            lines.append(line[st:])
            return lines
        nd = st + 60
        while line[nd-1] != " ":
            nd = nd - 1
            if nd == st:
                raise Exception, "can't handle no spaces"
        while line[nd-1] == " ": nd = nd - 1 # back up to no spaces
        if line.count("\"", st, nd) % 2 != 0:
            #pdb.set_trace()
            ix = line.rfind("\"", st, nd)
            if ix < 0: raise Exception, "chopup: bad line"
            if st + ix > mx/2:
                nd = ix
            else:
                # \fix should split quote
                nd = ix
        lines.append(line[st:nd])
        while nd < ln and line[nd] == " ": nd = nd + 1
        st = nd
    return lines

if True:
    enums,defs = slurp("cputildefs.h")
    enumd = get_enum_values("cputildefs.h", enums)
    defs = massage_defs(defs, enumd)
    #
    alldefs = []
    for r in defcombos:
        collect_qdefs(r, alldefs)
    #
    n = len(defcombos)
    xdefd = {}
    for i in range(n):
        dc = defcombos[i]
        xdefs = expand_defs(defs, dc, alldefs)
        xdefd[i] = xdefs
    fname = "cputil.h"
    replace_in_code((defcombos,xdefd), fname,  "autocoding", genxhdr1)
    move_if_changed(fname)

if False:
    for qc in defcombos:
        #v = qr_expand(qc)[1:-1]
        v = qr_mindefs(qc)
        print v

# --- last line of gen_header.py ---
