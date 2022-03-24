#ifndef THEME_EDITOR_PAGE_H
#define THEME_EDITOR_PAGE_H

#include <QWidget>

namespace Ui {
class theme_editor_page;
}

class theme_editor_page : public QWidget
{
    Q_OBJECT

public:
    explicit theme_editor_page(QWidget *parent = nullptr);
    ~theme_editor_page();

private:
    Ui::theme_editor_page *ui;
};

#endif // THEME_EDITOR_PAGE_H
