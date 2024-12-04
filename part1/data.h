#ifndef extern_
 #define extern_ extern
#endif

// Global variables
// Copyright (c) 2019 Warren Toomey, GPL3

// Global variables
// Copyright (c) 2019 Warren Toomey, GPL3 
// Since the extern in not defined here extern_ is extern here. This means the variable are declared here but the but then they will be defined exactly once outside this file.

extern_ int     Line;
extern_ int	Putback;
extern_ FILE	*Infile;
