/*********************************************************************************
**
** Copyright (c) 2017 The University of Notre Dame
** Copyright (c) 2017 The Regents of the University of California
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
** list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice, this
** list of conditions and the following disclaimer in the documentation and/or other
** materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its contributors may
** be used to endorse or promote products derived from this software without specific
** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
** SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
***********************************************************************************/

// Contributors:
// Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

#include <QApplication>
#include <QObject>
#include <QtGlobal>
#include <QFile>
#include <string.h>
#include "vwtinterfacedriver.h"

#include <QSslSocket>

void emptyMessageHandler(QtMsgType, const QMessageLogContext &, const QString &){}

int main(int argc, char *argv[])
{
    QApplication mainRunLoop(argc, argv);
    VWTinterfaceDriver programDriver;

    QFile styleFile(":/cweStyle.qss");

    if (!styleFile.open(QFile::ReadOnly))
    {
        programDriver.fatalInterfaceError("Unable to open style file. Install may be corrupted.");
    }
    QString styleText(styleFile.readAll());
    mainRunLoop.setStyleSheet(styleText);

    bool debugLoggingEnabled = false;
    bool runOffline = false;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i],"enableDebugLogging") == 0)
        {
            debugLoggingEnabled = true;
        }
        if (strcmp(argv[i],"offlineMode") == 0)
        {
            runOffline = true;
        }
    }

    if (runOffline)
    {
        qDebug("NOTE: Running CWE client offline.");
    }

    if (debugLoggingEnabled)
    {
        qDebug("NOTE: Debugging text output is enabled.");
    }
    else
    {
        qInstallMessageHandler(emptyMessageHandler);
    }

    mainRunLoop.setQuitOnLastWindowClosed(false);
    //Note: Window closeing must link to the shutdown sequence, otherwise the app will not close
    //Note: Might consider a better way of implementing this.

    if (QSslSocket::supportsSsl() == false)
    {
        programDriver.fatalInterfaceError("SSL support was not detected on this computer.\nPlease insure that some version of SSL is installed,\n such as by installing OpenSSL.");
    }


    if (runOffline)
    {
        programDriver.startOffline();
    }
    else
    {
        programDriver.startup();
    }

    return mainRunLoop.exec();
}
