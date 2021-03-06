﻿#include "LinesStatistics.h"

// Qt lib import
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonArray>

// JQLibrary import
#include "JQFile.h"

// JQToolsLibrary import
#include "JQToolsLibrary.hpp"

using namespace LinesStatistics;

QJsonObject Manage::statisticsLines(const QJsonArray &suffixs)
{
    auto fileCount = 0;
    auto lineCount = 0;

    auto currentPath = QFileDialog::getExistingDirectory( nullptr,
                                                          "Please choose code dir",
                                                          QStandardPaths::writableLocation( QStandardPaths::DesktopLocation ) );
    if( currentPath.isEmpty() )
    {
        return
            { {
                { "cancel", true }
            } };
    }

    QSet< QString > availableSuffixs;

    for ( const auto suffix: suffixs )
    {
        availableSuffixs.insert( suffix.toString().toLower() );
    }

    JQFile::foreachFileFromDirectory( { currentPath }, [ & ](const QFileInfo &info)
    {
        if( !availableSuffixs.contains( info.suffix().toLower() ) ) { return; }

        QFile file( info.filePath() );
        if( !file.open(QIODevice::ReadOnly) ) { return; }

        fileCount++;

        const auto &&fileAllData = file.readAll();

        if ( fileAllData.isEmpty() ) { return; }
        lineCount += fileAllData.count('\n') + 1;
    }, true);

    return
        { {
            { "fileCount", fileCount },
            { "lineCount", lineCount }
        } };
}
