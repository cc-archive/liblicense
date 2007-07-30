// Creative Commons has made the contents of this file
// available under a CC-GNU-LGPL license:
//
// http://creativecommons.org/licenses/LGPL/2.1/
//
// A copy of the full license can be found as part of this
// distribution in the file COPYING.
// 
// You may use the liblicense software in accordance with the
// terms of that license. You agree that you are solely 
// responsible for your use of the liblicense software and you
// represent and warrant to Creative Commons that your use
// of the liblicense software will comply with the CC-GNU-LGPL.
//
// Copyright 2007, Creative Commons, www.creativecommons.org.
// Copyright 2007, Jason Kivlighn.

#ifndef LICENSECHOOSER_H
#define LICENSECHOOSER_H

#include <kdebug.h>

#include <liblicense.h>

#include "ui_license_chooser.cpp"

class LicenseChooser : public QObject, public Ui::LicenseChooserWidget
{
    Q_OBJECT

public:
    LicenseChooser(QWidget *);
    virtual ~LicenseChooser();

signals:
    void licenseChanged();

private slots:
	void checkBoxClicked(QWidget *);

private:
	ll_license_chooser_t *chooser;
	int permits_flags;
	int requires_flags;
	int prohibits_flags;
};

#endif