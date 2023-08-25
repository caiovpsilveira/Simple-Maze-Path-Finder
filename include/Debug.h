/*
* Caio Vinicius Pereira Silveira
* caiovpsilveira@gmail.com
* Maze path finder algorithm - Lab. AI
* CEFET-MG
* August 2023
*/

#ifndef __DEBUG
#define __DEBUG

// compile with "-D DEBUG" to enable

#ifdef DEBUG

#include <iostream>

#define DEBUG_PRINTLN(x) do { std::cout << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ << "(): " << x << std::endl; } while (0)
#define DEBUG_CODE(x) do { x } while(0)
#else 
#define DEBUG_PRINTLN(x)
#define DEBUG_CODE(x)
#endif

#endif // __DEBUG