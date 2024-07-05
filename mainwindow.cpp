#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include <QDebug>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonCalculate, &QPushButton::clicked, this, &MainWindow::calculateZeroOneCount);
    connect(ui->pushButtonSwap, &QPushButton::clicked, this, &MainWindow::swapZerosAndOnes);
    connect(ui->pushButtonReverse, &QPushButton::clicked, this, &MainWindow::reverseMatrix);
    connect(ui->pushButtonMirror, &QPushButton::clicked, this, &MainWindow::mirrorMatrix);
    connect(ui->pushButtonBorder, &QPushButton::clicked, this, &MainWindow::addBorder);
    connect(ui->pushButtonFill, &QPushButton::clicked, this, &MainWindow::fillMatrix);
    connect(ui->pushButtonEdge, &QPushButton::clicked, this, &MainWindow::findEdges);
    connect(ui->pushButtonFindMatch, &QPushButton::clicked, this, &MainWindow::findMatch);
    connect(ui->pushButtonSave, &QPushButton::clicked, this, &MainWindow::saveMatrix);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateZeroOneCount()
{
    QString matrixText = getMatrixText();
    int zeroCount = matrixText.count('0');
    int oneCount = matrixText.count('1');
    ui->labelResult->setText("Zeros: " + QString::number(zeroCount) + ", Ones: " + QString::number(oneCount));
}

void MainWindow::swapZerosAndOnes()
{
    QString matrixText = getMatrixText();
    matrixText.replace('0', '2');
    matrixText.replace('1', '0');
    matrixText.replace('2', '1');
    ui->textEditMatrix->setPlainText(matrixText);
}

void MainWindow::reverseMatrix()
{
    QString matrixText = getMatrixText();
    QStringList rows = matrixText.split("\n", Qt::SkipEmptyParts);
    std::reverse(rows.begin(), rows.end());
    matrixText = rows.join("\n");
    ui->textEditMatrix->setPlainText(matrixText);
}

void MainWindow::mirrorMatrix()
{
    QString matrixText = getMatrixText();
    QStringList rows = matrixText.split("\n", Qt::SkipEmptyParts);

    for (int i = 0; i < rows.size(); ++i) {
        QString reversedRow;
        for (int j = rows[i].size() - 1; j >= 0; --j) {
            reversedRow.append(rows[i][j]);
        }
        rows[i] = reversedRow;
    }

    matrixText = rows.join("\n");
    ui->textEditMatrix->setPlainText(matrixText);
}

void MainWindow::addBorder()
{
    bool ok;
    int borderThickness = ui->lineEditBorderThickness->text().toInt(&ok);
    if (!ok || borderThickness <= 0) {
        ui->labelResult->setText("Invalid border thickness!");
        return;
    }

    QString matrixText = getMatrixText();
    QStringList rows = matrixText.split("\n", Qt::SkipEmptyParts);

    if (rows.isEmpty()) {
        ui->labelResult->setText("Matrix is empty!");
        return;
    }

    QString borderRow = QString(rows[0].size() + 2 * borderThickness, '2');
    QString newMatrixText;

    for (int i = 0; i < borderThickness; ++i) {
        newMatrixText.append(borderRow + "\n");
    }

    for (const QString& row : rows) {
        QString newRow = QString(borderThickness, '2') + row + QString(borderThickness, '2');
        newMatrixText.append(newRow + "\n");
    }

    for (int i = 0; i < borderThickness; ++i) {
        newMatrixText.append(borderRow + "\n");
    }

    ui->textEditMatrix->setPlainText(newMatrixText.trimmed());
    ui->labelResult->setText("Border added successfully.");
}

void MainWindow::fillMatrix()
{
    QString matrixText = getMatrixText();
    QStringList rows = matrixText.split("\n", Qt::SkipEmptyParts);

    for (int i = 0; i < rows.size(); ++i) {
        QString& row = rows[i];
        for (int j = 0; j < row.size(); ++j) {
            if (row[j] == '0') {
                if (i > 0 && rows[i - 1][j] == '1') {
                    row[j] = '3';
                }
                else if (i < rows.size() - 1 && rows[i + 1][j] == '1') {
                    row[j] = '3';
                }
                else if (j > 0 && row[j - 1] == '1') {
                    row[j] = '3';
                }
                else if (j < row.size() - 1 && row[j + 1] == '1') {
                    row[j] = '3';
                }
            }
        }
    }

    matrixText = rows.join("\n");
    ui->textEditMatrix->setPlainText(matrixText);
}

QString MainWindow::getMatrixText() const
{
    return ui->textEditMatrix->toPlainText();
}



QString MainWindow::horizontalEdges(const QStringList& rows)
{
    QString edgeImage;
    for (int i = 0; i < rows.size(); ++i) {
        QString currentRow = rows[i];
        QString nextRow = (i + 1 < rows.size()) ? rows[i + 1] : QString(currentRow.length(), '0'); // next row or padded with zeros

        for (int j = 0; j < currentRow.length(); ++j) {
            if (currentRow[j] != nextRow[j]) {
                edgeImage += '1';
            } else {
                edgeImage += '0';
            }
        }
        edgeImage += '\n';
    }
    return edgeImage.trimmed();
}

QString MainWindow::verticalEdges(const QStringList& rows)
{
    QString edgeImage;
    for (int i = 0; i < rows.size(); ++i) {
        QString currentRow = rows[i];

        for (int j = 0; j < currentRow.length(); ++j) {
            QChar currentPixel = currentRow[j];
            QChar nextPixel = (j + 1 < currentRow.length()) ? currentRow[j + 1] : '0'; // next pixel or padded with zero

            if (currentPixel != nextPixel) {
                edgeImage += '1';
            } else {
                edgeImage += '0';
            }
        }
        edgeImage += '\n';
    }
    return edgeImage.trimmed();
}
void MainWindow::findEdges()
{
    QString matrixText = ui->textEditMatrix->toPlainText().trimmed();
    QStringList rows = matrixText.split("\n", Qt::SkipEmptyParts);

    QString horizontalEdgesImage = horizontalEdges(rows);
    QString verticalEdgesImage = verticalEdges(rows);

    QString edgeImage;
    for (int i = 0; i < rows.size(); ++i) {
        QString row = rows[i];
        for (int j = 0; j < row.size(); ++j) {
            if (horizontalEdgesImage[i * (row.size() + 1) + j] == '1' || verticalEdgesImage[i * (row.size() + 1) + j] == '1') {
                edgeImage += '1';
            } else {
                edgeImage += '0';
            }
        }
        edgeImage += '\n';
    }

    ui->labelResult->setText(edgeImage.trimmed());
}

void MainWindow::findMatch()
{
    QString patternText = getPatternText();
    QString mainText = getMatrixText();

    if (patternText.isEmpty()) {
        ui->labelResult->setText("Pattern matrix is empty!");
        return;
    }

    QStringList mainRows = mainText.split("\n", Qt::SkipEmptyParts);

    QStringList patternRows = patternText.split("\n", Qt::SkipEmptyParts);

    int mainRowsCount = mainRows.size();
    int mainColsCount = (mainRowsCount > 0) ? mainRows[0].length() : 0;
    int patternRowsCount = patternRows.size();
    int patternColsCount = (patternRowsCount > 0) ? patternRows[0].length() : 0;

    if (patternRowsCount > mainRowsCount || patternColsCount > mainColsCount) {
        ui->labelResult->setText("Pattern matrix is larger than main matrix!");
        return;
    }

    bool patternFound = false;

    for (int i = 0; i <= mainRowsCount - patternRowsCount; ++i) {
        for (int j = 0; j <= mainColsCount - patternColsCount; ++j) {
            bool found = true;

            for (int r = 0; r < patternRowsCount; ++r) {
                for (int c = 0; c < patternColsCount; ++c) {
                    if (mainRows[i + r][j + c] != patternRows[r][c]) {
                        found = false;
                        break;
                    }
                }
                if (!found) break;
            }

            if (found) {
                for (int r = 0; r < patternRowsCount; ++r) {
                    mainRows[i + r].replace(j, patternColsCount, QString(patternColsCount, '2'));
                }

                patternFound = true;
                break;
            }
        }
        if (patternFound) break;
    }

    if (patternFound) {
        mainText = mainRows.join("\n");
        ui->textEditMatrix->setPlainText(mainText);
        ui->labelResult->setText("Pattern replaced with '2' in main matrix.");
    } else {
        ui->labelResult->setText("Pattern not found in main matrix!");
    }
}

QString MainWindow::getPatternText() const
{
    return ui->textEditPattern->toPlainText();
}

void MainWindow::saveMatrix()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Matrix"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing:" << fileName;
        return;
    }

    QTextStream out(&file);
    QString mainText = ui->textEditMatrix->toPlainText();
    out << mainText;

    file.close();
    qDebug() << "Matrix saved to file:" << fileName;
}
