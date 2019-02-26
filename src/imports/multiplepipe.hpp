#ifndef MULTIPLEPIPE_CPP
#define MULTIPLEPIPE_CPP

#include <vector>

using std::vector;

void multiplepipe(vector<char *> commands);
void exec_command(char **cmds, int current_cmd);
void pipe_command(char **cmds, int current_cmd, int *pipe_out);
bool ismultiplepipe(vector<char *> commands);

#endif
