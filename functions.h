#ifndef ISH_FUNCTIONS
#define ISH_FUNCTIONS

void pwd();
char *cd(char *command, char *path);
void Echo(char *command);
void ddir(char *path, int f_flag, int d_flag);
void dfile(char *path, char *filename, char *cwd, int f_flag, int d_flag);
void discover(char *path, char *command);
char *executeCommand(char *command, char *path);
int max(int a, int b);
void PrintStringArray(char** array, int n);
void PrintNumArray(char *array, int n);
int HasQuotes(char *word);
char *removeQuotes(char *word);
char *Dotify(char *path);
char *DeDotify(char *path);
char *AbsPathofDir(char *leading, char *pwd);
char *Dashify(char *command);
char *DeDashify(char *command);
int CheckValidDir(char *dir, char *path);
int CheckValidFile(char *file, char *path);
char *Tildify(char *path);
char *DeTildify(char *path);
char **AmpersandHandler(char *command);
void createHistory();
void history();
void AddHistory(char *command);
void init();
void jobs(char* command);
signed cmp(const void *a, const void *b);
char *lscolourblue(char *file);
char *lscolourwhite(char *file);
char *lscolourgreen(char *file);
char *lscolouryellow(char *file);
void lssize(char* dir, int lsl_flag, int lsa_flag);
void lsdir(char *dir, int lsl_flag, int lsa_flag);
void lsfile(char *file, int lsl_flag, int lsa_flag);
int lscmp(const void* x, const void* y);
void ls(char *path, char *command);
void pinfo(char *command);
void foregroundProcess(char *path, char *arguements[], int argc);
void backgroundProcess(char *path, char *arguements[], int argc);
void systemCommand(char *path, char *command);
int LookforBG();
void KillAllBG();
char* AmpersandSpacer(char * command);
void ResetStreams();
char* HandleRedirection(char* command);
void controlZ(int signum) ;
void controlC(int signum) ;
void bindSignals();
void sig(char* command);

#endif