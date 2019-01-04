# Makefile for Edge-Removal
# Last updated: February 12, 2016
# Modified by: Sammy Stolzenbach and Sam Swoap


############## Variables ###############
CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I. -I/comp/40/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/lib64 -L/usr/sup/cii40/lib64

# Libraries needed for linking
# Both programs need cii40 (Hanson binaries) and *may* need -lm (math)
LDLIBS = -lpnmrdr -lcii40 -lm

# Collect all .h files in your directory.
# This way, you can never forget to add
# a local .h file in your dependencies.
INCLUDES = $(shell echo *.h)

############### Rules ###############

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.

%.o: %.c $(INCLUDES)
        $(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)
unblackedges: edge_removal.o bit2.o 
        $(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)


clean:
        rm -f edge_removal *.o
