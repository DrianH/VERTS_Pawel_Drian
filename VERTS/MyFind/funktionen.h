//Gibt die gefundene Datei aus.
void printFile(pid_t pID, char *fileName, char *absPath);

//Gibt den absoluten Pfad einer Datei aus.
char *getAbsPath(const char *path);

//Zwei Strings werden verglichen abhaengig von caseIns.
int cmpFileNames(char *str1, char *str2, int caseIns);

//oldPath und fileName werden kombiniert.
char *buildNewPath(const char *oldPath, char *fileName);

//Es wird nach einer Datei in dir gesucht. Wenn rec zugewiesen ist, dann recursiv bei den unteren Ordnern auch suchen.
void searchFile(const char *dir, char *toSearch, int rec, int caseIns);

//In einem neuen Prozess einer Datei suchen.
pid_t forkSearch(const char *dir, char *toSearch, int rec, int caseIns);