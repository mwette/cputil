#!/usr/bin/python
#
# gen_tables.py - generate blank table or load (default) table into xp_main.c
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
# v130302b

import sys, os, re
from string import join, whitespace
import pdb
from gen_utils import write_w_fill, replace_in_code, move_if_changed

#typedef
#   enum
#   {
#      Iop_INVALID=0x12345, Iop_First, ...
#   }
#   IROp;

rx0 = re.compile(r"^((?:[^/]|(?:/[^/]))*)//.*")
rx1c = re.compile(r"^((?:[^*]|(?:\*[^/]))*)(/\*(?:[^*]|(?:\*[^/]))*\*/)(.*)$")

def readin_ir(name):
    """
    Read libvex_ir.h and load lines from guts of the Iop enum to get all ops.
    """
    startname = name + "_INVALID"
    lines = []
    f0 = open("../VEX/pub/libvex_ir.h", "r")
    l0 = f0.readline()
    while l0:
        l = l0.strip()
        if l.find(startname) >= 0:
             break
        l0 = f0.readline()
    while l0:
        l = l0.strip()
        if l.startswith("}"):
            break
        m = rx0.match(l)
        if m:
            l = m.group(1)
        lines.append(l)
        l0 = f0.readline()
    f0.close()
    return lines

def parse_enum(lines):
    """
    Given lines from enum guts, return list of enum names. 
    """
    text0 = join(lines)
    while True:
        m = rx1c.match(text0)
        if not m: 
            break
        text0 = m.group(1) + m.group(3)
    text1 = ""
    for c in text0:
        if c in whitespace:
            continue
        else:
            text1 += c
    enums = text1.split(",")
    return enums

def get_enums(name):
    lines = readin_ir(name)
    enums = parse_enum(lines)
    #
    e,val0 = enums[0].split("=")
    enums[0] = e
    return enums

def dump_blank_counts(lenums, oenums, filename):
    f1 = open(filename, 'w')
    f1.write("# valgrind/cputil count table\n")
    f1.write("#\n")
    f1.write("# load types\n")
    for e in lenums[1:]:
        f1.write("%d\t%s\n" % (0, e))
    f1.write("# other ops\n")
    for e in oenums[1:]:
        f1.write("%d\t%s\n" % (0, e))
    f1.close()

def read_counts(filename, lenums, oenums):
    """
    Read lines from filename.
    Skip until first lenum found, then match lines to lenum names.
    Skip until first oenum found, then match lines to oenum names.
    If mismatch, raise exception.  The lines should be of the form:
      # <comment>
    or
      <count> <opname>
    Return two lists: [(ldop,val),...],[(enum, val),...]
    The list elements are tuples of (string,string).
    """
    nld = len(lenums)
    nop = len(oenums)
    ld_pairs = []
    op_pairs = []
    #
    f0 = open(filename, "r")
    #
    l0 = f0.readline()
    while l0:
        if l0[0] == '#':
            l0 = f0.readline()
            continue
        c,s = l0.split()
        if s == lenums[1]:              # skip INVALID = lenums[0]
            break
        l0 = f0.readline()
    ld_pairs = [(lenums[0], "0")]
    i = 1                               # skip INVALID
    while l0:
        c,s = l0.split()
        if s != lenums[i]:
            raise Exception, "mismatch: " + s
        ld_pairs.append((s,c))
        i = i + 1
        if i >= nld:
            break
        l0 = f0.readline()
    if len(ld_pairs) != nld:
        raise Exception, "count mismatch"
    #
    l0 = f0.readline()
    while l0:
        if l0[0] == '#':
            l0 = f0.readline()
            continue
        c,s = l0.split()
        if s == oenums[1]:              # skip INVALID = lenums[0]
            break
        l0 = f0.readline()
    op_pairs = [(oenums[0], "0")]
    i = 1                               # skip INVALID
    while l0:
        c,s = l0.split()
        if s != oenums[i]:
            raise Exception, "mismatch: " + s
        op_pairs.append((s,c))
        i = i + 1
        if i >= nop:
            break
        l0 = f0.readline()
    if len(op_pairs) != nop:
        pdb.set_trace()
        raise Exception, "count mismatch"
    #
    f0.close()
    #
    return (ld_pairs, op_pairs)

def fill_names(tag, names, f1):
    """
    Write out list of enum names as array of C-strings.
    """
    f1.write("#define NUM_%s %d\n" % (tag.upper(), len(names)))
    if not names[0].endswith("INVALID"):
        raise Exception, "expecting INVALID as first element"
    f1.write("static Char *xp_%s_names[] = {\n  " % (tag,))
    col = 2
    for e in names:
        col = write_w_fill(f1, col, " \"%s\"," % (e,))
    f1.write("\n};\n")

def fill_counts(tag, vals, f1):
    """
    Write out count array.
    """
    f1.write("static UShort xp_%s_counts[] = {\n  " % (tag,))
    col = 2
    for c in vals:
        col = write_w_fill(f1, col, " %s," % (c,))
    f1.write("\n};\n")

def fill_all(pairs, f1):
    lpairs, opairs = pairs
    fill_names("op", map(lambda p: p[0], opairs), f1)
    f1.write("\n")
    fill_counts("op", map(lambda p: p[1], opairs), f1)
    f1.write("\n")
    fill_names("ld", map(lambda p: p[0], lpairs), f1)
    f1.write("\n")
    fill_counts("ld", map(lambda p: p[1], lpairs), f1)

# ========================================

ld_enums = None
op_enums = None

def get_all_enums():
    global ld_enums, op_enums
    if not op_enums:
        op_enums = get_enums("Iop")
    if not ld_enums:
        ld_enums = get_enums("Ity")

from getopt import getopt

if True:
    opts, args = getopt(sys.argv[1:], "d")
    for opt in opts:
        k,v = opt
        if k == "-d":
            print "dumping to xp_zero.cnt.new ..."
            get_all_enums()
            dump_blank_counts(ld_enums, op_enums, "xp_zero.cnt.new")
    else:
        print "updating cu_main.c ..."
        get_all_enums()
        pairs = read_counts("cu_def.cnt", ld_enums, op_enums)
        replace_in_code(pairs, "cu_main.c", "count_tables", fill_all)
        #lpairs, opairs = read_counts("cu_def.cnt", ld_enums, op_enums)
        #replace_in_code((lpairs,opairs), "cu_main.c", "count_tables", fill_all)
        res = move_if_changed("cu_main.c")
        if res:
            print "cu_main.c changed"
        else:
            print "cu_main.c not changed"

# --- last line of gen_tables.py ---
