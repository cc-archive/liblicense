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

#include "license_chooser.h"

#include <QtCore/QSignalMapper>
#include <kdebug.h>

#include <liblicense.h>

#ifdef HAVE_CONFIG_H
#include "../../../config.h"
#endif

LicenseChooser::LicenseChooser( QWidget *parent )
{
	setupUi(parent);

	// CLEANUP: icon paths
	attributionCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/by.svg") );
	sharingCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/ash.svg") );
	remixingCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/ar.svg") );
	noCommercialCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/pcw.svg") );
	shareAlikeCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/sa.svg") );

	connect(uriEdit,SIGNAL(textChanged(const QString &)),this,SLOT(setDirty()));

	QSignalMapper *signalMapper = new QSignalMapper(this);

	connect(attributionCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
	connect(sharingCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
	connect(remixingCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
	connect(noCommercialCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
	connect(shareAlikeCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));

	signalMapper->setMapping(attributionCheckBox, attributionCheckBox);
	signalMapper->setMapping(sharingCheckBox, sharingCheckBox);
	signalMapper->setMapping(remixingCheckBox, remixingCheckBox);
	signalMapper->setMapping(noCommercialCheckBox, noCommercialCheckBox);
	signalMapper->setMapping(shareAlikeCheckBox, shareAlikeCheckBox);

	connect(signalMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(checkBoxClicked(QWidget*)));
}

LicenseChooser::~LicenseChooser()
{
}

void LicenseChooser::checkBoxClicked(QWidget *checkBox)
{
	kdDebug() << checkBox->objectName() << " clicked" << endl;

	emit licenseChanged();
}
