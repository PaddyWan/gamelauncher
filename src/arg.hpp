#ifndef ARG_HPP
#define ARG_HPP

//will parse arguments
// returns 0 if nothing special must be done
// returns 1 if the program must exit with return code 0
// returns anything else, the program must exit with that return code
int parse_arg(int argc, char** argv);

#endif//ARG_HPP
