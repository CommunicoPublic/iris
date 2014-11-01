/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#define C_SERVER_PORT 2000

// C++ Includes
#include "ClientTCP4Socket.hpp"
#include "ServerTCP4Socket.hpp"
#include "SignalHandler.hpp"
#include "SocketPoller.hpp"
#include "Debug.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

class TestSignalHandler:
  public UserSignalHandler
{
public:
	/**
	  @brief A signal handler
	  @param iSignal - signal number
	*/
	void Handler(INT_32 iSignal);

	/**
	  @brief A Virtual Destructor
	*/
	~TestSignalHandler() throw();
};

//
// A signal handler
//
void TestSignalHandler::Handler(const INT_32 iSignal)
{
	switch (iSignal)
	{
		case SIGCHLD:
			{
				int iStatus;
				pid_t iKilledPid = 0;
				while ((iKilledPid = waitpid(-1, &iStatus, WNOHANG)) > 0)
				{
					if      (WIFEXITED(iStatus))   { fprintf(stdout, "%d catched SIGCHIHLD. Pid is %d, status %d\n",           INT_32(getpid()), INT_32(iKilledPid), WEXITSTATUS(iStatus)); }
					else if (WIFSIGNALED(iStatus)) { fprintf(stdout, "%d catched SIGCHIHLD. Pid is %d, killed by signal %d\n", INT_32(getpid()), INT_32(iKilledPid), WTERMSIG(iStatus));    }
				}
			}
			break;

		case SIGINT:
			fprintf(stdout, "%d catched SIGINT\n", INT_32(getpid()));
			exit(EX_OK);
			break;

		case SIGTERM:
			fprintf(stdout, "%d catched SIGTERM\n", INT_32(getpid()));
			exit(EX_OK);
			break;

		default:
			fprintf(stdout, "%d unknown signal catched: %d\n", INT_32(getpid()), iSignal);
	}
}

//
// A Destructor
//
TestSignalHandler::~TestSignalHandler() throw() { ;; }

static CCHAR_P aDecalogus[] =
{
	"Non habebis deos alienos in conspectu meo. Non facies tibi sculptile nec similitudinem omnium, quae in caelo sunt desuper et quae in terra deorsum et quae versantur in aquis sub terra. Non adorabis ea et non coles: Ego enim sum Dominus Deus tuus, Deus aemulator, reddens iniquitatem patrum super filios in tertiam et quartam generationem his, qui oderunt me, et faciens misericordiam in multa milia diligentibus me et custodientibus praecepta mea.",
	"Non usurpabis nomen Domini Dei tui frustra, quia non erit impunitus, qui super re vana nomen eius assumpserit.",
	"Observa diem sabbati, ut sanctifices eum, sicut praecepit tibi Dominus Deus tuus. Sex diebus operaberis et facies omnia opera tua. Septimus dies sabbatum est Domino Deo tuo. Non facies in eo quidquam operis tu et filius tuus et filia, servus et ancilla et bos et asinus et omne iumentum tuum et peregrinus tuus, qui est intra portas tuas, ut requiescat servus tuus et ancilla tua sicut et tu. Memento quod et ipse servieris in Aegypto, et eduxerit te inde Dominus Deus tuus in manu forti et brachio extento: idcirco praecepit tibi, ut observares diem sabbati.",
	"Honora patrem tuum et matrem, sicut praecepit tibi Dominus Deus tuus, ut longo vivas tempore et bene sit tibi in terra, quam Dominus Deus tuus daturus est tibi.",
	"Non occides.",
	"Neque moechaberis.",
	"Furtumque non facies.",
	"Nec loqueris contra proximum tuum falsum testimonium.",
	"Nec concupisces uxorem proximi tui.",
	"Nec desiderabis domum proximi tui, non agrum, non servum, non ancillam, non bovem, non asinum et universa, quae illius sunt.",
	"Dixi",
	NULL
};

static void SpawnServer(const INT_32 iServerPort)
{
	ClientTCPSocket * pClientSocket = new ClientTCP4Socket("127.0.0.1", iServerPort);

	pClientSocket -> Open();

	pClientSocket -> SetMode(TCPSocket::BLOCKING);
	pClientSocket -> SetMode(TCPSocket::CLOSE_ON_EXEC);

	pClientSocket -> SetFlags(TCPSocket::KEEPALIVE, 1);
	pClientSocket -> SetFlags(TCPSocket::REUSE_ADDRESS, 1);

	dynamic_cast<ClientTCP4Socket *>(pClientSocket) -> Connect();

	// Poller for client socket
	SocketPoller<ClientTCPSocket *> oSocketPoller(1);
	oSocketPoller.AddSocket(pClientSocket, pClientSocket, POLLOUT);

	CCHAR_P * aDecalogusIt = aDecalogus;
	for (;;)
	{
		CCHAR_P sData    = *aDecalogusIt;
		UINT_64 iDataLen = strlen(sData);
		UINT_64 iTotalWrote = 0;
		for(;;)
		{
			const TCPSocket::State oRC = oSocketPoller.Poll(-1);
			if (oRC != TCPSocket::OK)
			{
				INT_32 iErrNo = oSocketPoller.GetError();

				fprintf(stderr, "POLL/ERROR: %s", Debug::PrintableError(iErrNo).c_str());
				break;
			}

			UINT_32 iReadySockets = 0;
			SocketPoller<ClientTCPSocket *>::SocketData * aSockets = oSocketPoller.GetReadySockets(iReadySockets);
			if (iReadySockets == 1)
			{
				UINT_64 iWroteBytes = 0;
				dynamic_cast<ClientTCPSocket *>(aSockets[0].socket) -> Write(sData + iTotalWrote, iDataLen - iTotalWrote, iWroteBytes);
				iTotalWrote += iWroteBytes;
			}
			if (iTotalWrote == iDataLen) { break; }
		}

		++aDecalogusIt;
		if (*aDecalogusIt == NULL) { break; }
	}
	delete pClientSocket;
}

int main(int argc, char ** argv)
{
	// Create Signal handler singletone
	SignalHandler & oSigHandler = SignalHandler::Instance();

	TestSignalHandler oTestSignalHandler;

	oSigHandler.RegisterHandler(SIGCHLD, &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGINT,  &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGTERM, &oTestSignalHandler);
	oSigHandler.RegisterHandler(SIGUSR1, &oTestSignalHandler);

	pid_t iPid1 = fork();
	if (iPid1 == 0)
	{
		sleep(1);
		SpawnServer(C_SERVER_PORT);

		fprintf(stderr, "%d exited\n", INT_32(getpid()));
		return EX_OK;
	}

	ServerTCPSocket * pServerSocket = new ServerTCP4Socket("127.0.0.1", C_SERVER_PORT);

	pServerSocket -> Open();

	pServerSocket -> SetMode(TCPSocket::BLOCKING);
	pServerSocket -> SetMode(TCPSocket::CLOSE_ON_EXEC);

	pServerSocket -> SetFlags(TCPSocket::KEEPALIVE, 1);
	pServerSocket -> SetFlags(TCPSocket::REUSE_ADDRESS, 1);

	pServerSocket -> Bind();
	pServerSocket -> Listen(128);

	ClientTCPSocket * pClientSocket = NULL;
	pServerSocket -> Accept(pClientSocket);

	// Poller for client socket
	SocketPoller<ClientTCPSocket *> oSocketPoller(1);
	if (pClientSocket)
	{
		pClientSocket -> SetMode(TCPSocket::NON_BLOCKING);
		pClientSocket -> SetMode(TCPSocket::CLOSE_ON_EXEC);

		pClientSocket -> SetFlags(TCPSocket::KEEPALIVE, 1);
		pClientSocket -> SetFlags(TCPSocket::REUSE_ADDRESS, 1);

		oSocketPoller.AddSocket(pClientSocket, pClientSocket, POLLIN);

		for(;;)
		{
			CHAR_8 sBuffer[50];
			UINT_64 iReadBytes = 0;

			const TCPSocket::State oRC = oSocketPoller.Poll(-1);
			if (oRC != TCPSocket::OK)
			{
				INT_32 iErrNo = oSocketPoller.GetError();
				fprintf(stderr, "POLL/ERROR: %s", Debug::PrintableError(iErrNo).c_str());
				break;
			}

			UINT_32 iReadySockets = 0;
			SocketPoller<ClientTCPSocket *>::SocketData * aSockets = oSocketPoller.GetReadySockets(iReadySockets);
			if (iReadySockets == 1)
			{
				dynamic_cast<ClientTCPSocket *>(aSockets[0].socket) -> Read(sBuffer, 50, iReadBytes);
			}
			if (iReadBytes == 0) { break; }
			fprintf(stderr, "%.*s\n", INT_32(iReadBytes), sBuffer);
		}

		pClientSocket -> Close();
		delete pClientSocket;
	}

	delete pServerSocket;
	// Wait for child
	fprintf(stderr, "%d waiting for %d\n\n", INT_32(getpid()), INT_32(iPid1));
	while (sleep(1) != 0) { ;; }

	fprintf(stderr, "All done: %d\n", INT_32(getpid()));

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
