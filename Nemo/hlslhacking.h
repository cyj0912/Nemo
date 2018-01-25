#ifndef HLSLHACKING_H
#define HLSLHACKING_H

#include <QWidget>

namespace Ui {
class HlslHacking;
}

class HlslHacking : public QWidget
{
    Q_OBJECT

public:
    explicit HlslHacking(QWidget *parent = 0);
    ~HlslHacking();

private:
    Ui::HlslHacking *ui;
};

#endif // HLSLHACKING_H
