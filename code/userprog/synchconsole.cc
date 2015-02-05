#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"




static Semaphore *readAvail;
static Semaphore *writeDone;

static Semaphore *writeChar;
static Semaphore *readChar;



static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0); // Console asynchronous
	
	// Un seul processus autorisé à lire et écrire dans la console
	// Exlusion mutuelle
	writeChar = new Semaphore("write char",1);
	readChar = new Semaphore("read char",1);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	//Principe du sémaphore
	writeChar->P(); // On prend la clé, donc blocage pour les autres processus 

	console->PutChar(ch); // Ecriture 
	
	writeDone->P(); // On bloque quand on a fini d'écrire

	writeChar->V(); // On libere la ressource
}

char SynchConsole::SynchGetChar()
{
	// Même principe que pour le SynchPutChar
	readChar->P(); 
	
	readAvail->P();
	
	char c = console->GetChar(); // on sauvegarde la lettre

	readChar->V();
	return c;
}

void SynchConsole::SynchPutString(const char s[])
{
// ...
}

void SynchConsole::SynchGetString(char *s, int n)
{
// ...
}
#endif // CHANGED
