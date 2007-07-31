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
#include <klocale.h>

#include <liblicense.h>

#ifdef HAVE_CONFIG_H
#include "../../../config.h"
#endif

char *attributes[] = {
		"http://creativecommons.org/ns#Distribution",
		"http://creativecommons.org/ns#CommercialUse",
		"http://creativecommons.org/ns#DerivativeWorks",
		"http://creativecommons.org/ns#ShareAlike",
		"http://creativecommons.org/ns#Attribution",
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

	permits_flags = requires_flags = prohibits_flags = LL_UNSPECIFIED;

	jurisdictionComboBox->addItem(QIcon(LICENSE_DIR "../icons/unported.png"), i18n("Unported"), QVariant(QString::null));
	jurisdictionComboBox->addItem(QIcon(LICENSE_DIR "../icons/uk.png"), i18n("United Kingdom"), QVariant("uk"));
	jurisdictionComboBox->addItem(QIcon(LICENSE_DIR "../icons/us.png"), i18n("United States"), QVariant("us"));

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

	connect(jurisdictionComboBox, SIGNAL(activated(int)), this, SLOT(updateJurisdiction()));

	updateChooser();
}

LicenseChooser::~LicenseChooser()
{
	ll_free_license_chooser(chooser);
	ll_stop();
}

QString LicenseChooser::licenseURI()
{
	return uriEdit->text();
}

void LicenseChooser::setLicenseURI( const QString &uriString )
{
	int flag;
	char **attr;
	char **attrs;

	const QByteArray uri_ba = uriString.toUtf8();
	const uri_t uri = (const uri_t)uri_ba.data();

	if ( !ll_verify_uri(uri) ) {
		uriEdit->setText(QString::fromUtf8(uri));
		return;
	}

	attrs = ll_get_permits(uri);
	for (attr=attrs; *attr; ++attr) {
		if (strcmp(*attr,"http://creativecommons.org/ns#Distribution")==0) {
			sharingCheckBox->setChecked(true);
		} else if (strcmp(*attr,"http://creativecommons.org/ns#DerivativeWorks")==0) {
			remixingCheckBox->setChecked(true);
		}
	}

	attrs = ll_get_requires(uri);
	for (attr=attrs; *attr; ++attr) {
		if (strcmp(*attr,"http://creativecommons.org/ns#Attribution")==0) {
			attributionCheckBox->setChecked(true);
		} else if (strcmp(*attr,"http://creativecommons.org/ns#ShareAlike")==0) {
			shareAlikeCheckBox->setChecked(true);
		}
	}

	attrs = ll_get_prohibits(uri);
	for (attr=attrs; *attr; ++attr) {
		if (strcmp(*attr,"http://creativecommons.org/ns#CommercialUse")==0) {
			noCommercialCheckBox->setChecked(true);
		}
	}

	char *j = ll_get_jurisdiction(uri);
	QString juris = QString::fromLatin1(j);
	int i;
	for (i=0; i<jurisdictionComboBox->count(); ++i) {
		if (juris == jurisdictionComboBox->itemData(i).toString()) {
			jurisdictionComboBox->setCurrentIndex(i);
			updateChooser();
			goto juris_found;
		}
	}
	//no match found, set to unported
	if ( jurisdictionComboBox->currentIndex() != 0 ) {
		jurisdictionComboBox->setCurrentIndex(0);
		updateChooser();
	}

	juris_found:
	free(j);

	updateLicense(uri);
}

void LicenseChooser::updateJurisdiction()
{
	updateChooser();
	updateLicense();
}

void LicenseChooser::updateChooser()
{
	QByteArray juris_ba = jurisdictionComboBox->itemData( jurisdictionComboBox->currentIndex() ).toString().toLatin1();
	char *juris = juris_ba.data();
	if ( juris[0] == '\0' ) juris = NULL;

	kdDebug() << "juris: " << juris << endl;
	chooser = ll_new_license_chooser(juris,attributes);
}

void LicenseChooser::updateLicense()
{
	print_flags(attributes,permits_flags,requires_flags,prohibits_flags);
	const ll_license_list_t *licenses = ll_get_licenses_from_flags(chooser,permits_flags,requires_flags,prohibits_flags);
	if (licenses) {
		const uri_t uri = (const uri_t)licenses->license;
		updateLicense(uri);
	} else {
		kdDebug() << "No license matches" << endl;
		uriEdit->setText(QString::null);
		licenseEdit->setText(i18n("none"));
		emit licenseChanged();
	}
}

void LicenseChooser::updateLicense(const uri_t uri)
{
		uriEdit->setText(QString::fromLatin1(uri));

		int *v = ll_get_version(uri);

		licenseEdit->setText(
			QString("%1 - %2.%3.%4")
				.arg(QString::fromLatin1(ll_get_name(uri)))
				.arg(v[0]).arg(v[1]).arg(v[2])
		);
		free(v);
}

void LicenseChooser::checkBoxClicked(QWidget *checkBox)
{
	kdDebug() << checkBox->objectName() << " clicked" << endl;
	if ( checkBox == attributionCheckBox ) {
		requires_flags ^= ll_attribute_flag(chooser,"http://creativecommons.org/ns#Attribution");
	} else if ( checkBox == sharingCheckBox ) {
		permits_flags ^= ll_attribute_flag(chooser,"http://creativecommons.org/ns#Distribution");
	} else if ( checkBox == remixingCheckBox ) {
		permits_flags ^= ll_attribute_flag(chooser,"http://creativecommons.org/ns#DerivativeWorks");

		shareAlikeCheckBox->setEnabled( remixingCheckBox->isChecked() );
		if (!remixingCheckBox->isChecked())
			shareAlikeCheckBox->setChecked(false);

	} else if ( checkBox == noCommercialCheckBox ) {
		prohibits_flags ^= ll_attribute_flag(chooser,"http://creativecommons.org/ns#CommercialUse");
	} else if ( checkBox == shareAlikeCheckBox ) {
		requires_flags ^= ll_attribute_flag(chooser,"http://creativecommons.org/ns#ShareAlike");
	}

	updateLicense();
}
