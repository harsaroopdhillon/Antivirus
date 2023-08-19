#include "setting_page.h"
#include "setting.h"
#include "scan_setting.h"
#include "browserprotection.h"

setting_page::setting_page()
{
    wid_init();
    connections();
}

void setting_page :: wid_init()
{
    st = new setting();
    ss = new scan_setting();
    bp = new browserProtection();

    this->addWidget(st); this->addWidget(ss); this->addWidget(bp);
}

void setting_page :: gotoHome()
{
    this->setCurrentWidget(st);
}

void setting_page :: gotoSS()
{
    this->setCurrentWidget(ss);
}

void setting_page :: gotoBP()
{
    this->setCurrentWidget(bp);
}

void setting_page :: connections()
{
    connect(ss , SIGNAL(gotoHome()) , this , SLOT(gotoHome()));
    connect(bp , SIGNAL(gotoHome()) , this , SLOT(gotoHome()));
    connect(st, SIGNAL(scanner_req()) , this , SLOT(gotoSS()));
    connect(st , SIGNAL(browser_req()) , this , SLOT(gotoBP()));
}
