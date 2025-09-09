#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <stdlib.h>
#include <time.h>


/**
 * @brief generate random ascii string
 * 
 * @param length: length of string needed be generated(exclude '\0')
 * 
 * @return pointor to the str, [NULL] on failure
 */
char *genRandomAsciiStr(int length);


#endif /* _TOOLS_H_ */