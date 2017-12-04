#!/usr/bin/python

"""
Comm in python 
Compares two files line by line
Supports -u flag for unsorted files
"""

import random, sys
from optparse import OptionParser



class randline:
    def __init__(self, filename, filename2, delim):
        try:
            if filename == "-":
                f1 = sys.stdin
                f2 = open(filename2, 'r')
            elif filename2 == "-":
                f2 = sys.stdin
                f1 = open(filename, 'r')
            elif filename == "-" and filename2 == "-":
                print("Error! Can't read both files from STDIN")
                exit()
            else:
                f1 = open(filename, 'r')
                f2 = open(filename2, 'r')
            #f = open(filename, 'r')
            self.lines1 = f1.read().split(delim)
            f1.close()
            if self.lines1[-1]=="" :
                del self.lines1[-1]
            #f = open(filename2, 'r')
            self.lines2 = f2.read().split(delim)
            if self.lines2[-1]=="":
                del self.lines2[-1]
            f2.close()
            
        except IOError as e:
            errno = e.errno
            strerror = e.strerror
            parser.error("I/O error({0}): {1}".
                          format(errno, strerror))

    def chooseline(self):
        return random.choice(self.lines1)
    
    def s_comparelines(self, options):
        i=0
        j=0
        not_yet_error_1=True
        not_yet_error_2=True
        l1 = self.lines1
        l2 = self.lines2

        while (i<len(self.lines1) and j<len(self.lines2)):
            
            if (i!=0 and l1[i]<l1[i-1] and not_yet_error_1):
                sys.stderr.write("comm: file 1 is not in sorted order\n")
                not_yet_error_1=False
            if (j!=0 and l2[j]<l2[j-1] and not_yet_error_2):
                sys.stderr.write("comm: file 2 is not in sorted order\n")
                not_yet_error_2=False

            if l1[i]==l2[j]:
                if options.print_col_3:
                    printed=""
                    if options.print_col_1:
                        printed=printed+"\t"
                    if options.print_col_2:
                        printed=printed+"\t"
                    print (printed+l1[i])
                i=i+1
                j=j+1
            elif l1[i]<l2[j]:
                if options.print_col_1:
                    print (l1[i])
                i=i+1
            else:
                if options.print_col_2:
                    printed=""
                    if options.print_col_1:
                        printed="\t"
                    print (printed+l2[j])
                j=j+1

        while i<len(self.lines1):
            if (i!=0 and l1[i]<l1[i-1] and not_yet_error_1):
                sys.stderr.write("comm: file 1 is not in sorted order\n")
                not_yet_error_1=False
            if options.print_col_1:
                print (l1[i])
            i=i+1
        while j<len(self.lines2):
            if (j!=0 and l2[j]<l2[j-1] and not_yet_error_2):
                sys.stderr.write("comm: file 2 is not in sorted order\n")
                not_yet_error_2=False
            if options.print_col_2:
                printed=""
                if options.print_col_1:
                    printed="\t"
                print (printed+l2[j])
            j=j+1

    def u_comparelines(self, options):
        i=0
        j=0
        not_yet_error_1=True
        not_yet_error_2=True

        l1 = self.lines1
        l2 = self.lines2
        
        while (i<len(self.lines1)):
            try:
                j=l2.index(l1[i])  
                del l2[j]
                if options.print_col_3:
                    printed = ""
                    if options.print_col_1:
                        printed = "\t"
                    if options.print_col_2:
                        printed = printed + "\t" 
                    print (printed + l1[i])
            except ValueError:
                if options.print_col_1:
                    print (l1[i]    )
            i=i+1 
        

        j=0
        while j<len(self.lines2):
            if options.print_col_2:
                printed=""
                if options.print_col_1:
                    printed="\t"
                print (printed+l2[j])
            j=j+1

    def comparelines(self,options):
        if options.handle_unsorted:
            self.u_comparelines(options)
        else:
            self.s_comparelines(options)

def printlines(self):
    print (self.lines1, self.lines2)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
Compare sorted files FILE1 and FILE2 line by line.

When FILE1 or FILE2 (not both) is -, read standard input.

With no options, produce three-column output.  Column one contains
lines unique to FILE1, column two contains lines unique to FILE2,
and column three contains lines common to both files.

  -1              suppress column 1 (lines unique to FILE1)
  -2              suppress column 2 (lines unique to FILE2)
  -3              suppress column 3 (lines that appear in both files)

  --check-order     check that the input is correctly sorted, even
                      if all input lines are pairable
  --nocheck-order   do not check that the input is correctly sorted
  --output-delimiter=STR  separate columns with STR
  -z, --zero-terminated    line delimiter is NUL, not newline
      --help     display this help and exit
      --version  output version information and exit

Note, comparisons honor the rules specified by 'LC_COLLATE'.

Examples:
  comm -12 file1 file2  Print (only lines present in both file1 and file2.)
  comm -3 file1 file2  Print (lines in file1 not in file2, and vice versa.)

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
Full documentation at: <http://www.gnu.org/software/coreutils/comm>
or available locally via: info '(coreutils) comm invocation'"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", "--one", action="store_false",
        dest="print_col_1", default=True)
    parser.add_option("-2", action="store_false",
        dest="print_col_2", default=True)
    parser.add_option("-3", action="store_false",
        dest="print_col_3", default=True)
    parser.add_option("-z", "--zero-terminated", action="store_true",
        dest="zero_terminated",default=False)
    parser.add_option("-u", action="store_true",
        dest="handle_unsorted",default=False)
    options, args = parser.parse_args(sys.argv[1:])

  
    if len(args) != 2:
        parser.error("wrong number of operands")
    input_file = args[0]
    input_file_2 = args[1] 

    try:
        delim = "\n"
        if options.zero_terminated:
            delim = "\x00"
        generator = randline(input_file, input_file_2, delim)
        generator.comparelines(options)
    except IOError as errno_strerror:
        parser.error("I/O error({0}): {1}".
                     format(errno_strerror[0], errno_strerror[1]))

if __name__ == "__main__":
    main()