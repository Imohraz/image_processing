#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void findEdges();
    QString getPatternText() const;
    void saveMatrix();
private slots:
    QString horizontalEdges(const QStringList& rows);
    QString verticalEdges(const QStringList& rows);
    void calculateZeroOneCount();
    void swapZerosAndOnes();
    void reverseMatrix();
    void mirrorMatrix();
    void addBorder();
    void fillMatrix();
    QString getMatrixText() const;
    void findMatch();

private:
    Ui::MainWindow *ui;

};

#endif
