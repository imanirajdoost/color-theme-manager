#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <QWidget>

namespace Ui {
class home_page;
}

class home_page : public QWidget
{
    Q_OBJECT

public:
    explicit home_page(QWidget *parent = nullptr);
    ~home_page();

private:
    Ui::home_page *ui;
};

#endif // HOME_PAGE_H
