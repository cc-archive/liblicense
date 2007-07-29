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

char *attributes[] = {
		"http://creativecommons.org/ns#Distribution",			//0
		"http://creativecommons.org/ns#CommercialUse",		//1
		"http://creativecommons.org/ns#DerivativeWorks",	//2
		"http://creativecommons.org/ns#ShareAlike",				//3
		"http://creativecommons.org/ns#Attribution",			//4
		NULL
};

void print_flags( char **attributes, int p, int r, int pr )
{
	printf("Selected flags:\n");
	int i = 0;
	while (attributes[i]) {
		if (p & (1<<i)) {
			printf("\tPERMITS   : %s\n",attributes[i]);
		} else if (r & (1<<i)) {
			printf("\tREQUIRES  : %s\n",attributes[i]);
		} else if (pr & (1<<i)) {
			printf("\tPROHIBITS : %s\n",attributes[i]);
		}
		++i;
	}
	printf("\n");
}


LicenseChooser::LicenseChooser( QWidget *parent )
{
	setupUi(parent);

	ll_init();

	permits_flags = requires_flags = prohibits_flags = 0;


	chooser = ll_new_license_chooser(NULL,attributes);

	// CLEANUP: icon paths
	attributionCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/by.svg") );
	sharingCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/ash.svg") );
	remixingCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/ar.svg") );
	noCommercialCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/pcw.svg") );
	shareAlikeCheckBox->setIcon( QIcon(LICENSE_DIR "../icons/sa.svg") );

	connect(uriEdit,SIGNAL(textChanged(const QString &)),this,SIGNAL(licenseChanged()));

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
	ll_free_license_chooser(chooser);
	ll_stop();
}

void LicenseChooser::checkBoxClicked(QWidget *checkBox)
{
	kdDebug() << checkBox->objectName() << " clicked" << endl;
	if ( checkBox->objectName() == "attributionCheckBox" ) {
		requires_flags ^= (1 << 4);
	} else if ( checkBox->objectName() == "sharingCheckBox" ) {
		permits_flags ^= (1 << 0);
	} else if ( checkBox->objectName() == "remixingCheckBox" ) {
		permits_flags ^= (1 << 2);
	} else if ( checkBox->objectName() == "noCommercialCheckBox" ) {
		prohibits_flags ^= (1 << 1);
	} else if ( checkBox->objectName() == "shareAlikeCheckBox" ) {
		requires_flags ^= (1 << 3);
	}

	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	const ll_license_list_t *licenses = ll_get_licenses_from_flags(chooser,permits_flags,requires_flags,prohibits_flags);
	if (licenses->next) {
		kdDebug() << "Matching licenses\n" << endl;
		ll_license_list_t *curr = licenses->next;
		while (curr) {
			kdDebug() << "\t" << curr->license << endl;
			curr = curr->next;
		}

		const uri_t uri = (const uri_t)licenses->next->license;
		uriEdit->setText(QString::fromLatin1(uri));

		int *v = ll_get_version((const uri_t)uri);

		licenseEdit->setText(
			QString("%1 - %2.%3.%4")
				.arg(QString::fromLatin1(ll_get_name(uri)))
				.arg(v[0]).arg(v[1]).arg(v[2])
		);
	} else {
		kdDebug() << "No license matches" << endl;
		uriEdit->setText(QString::null);
		licenseEdit->setText(QString::null);
	}

	emit licenseChanged();
}
