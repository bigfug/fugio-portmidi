#include "portmidiplugin.h"

#include <QTranslator>
#include <QApplication>

#include "devicemidi.h"

#include <fugio/global_interface.h>
#include <fugio/global_signals.h>

#include "portmidiinputnode.h"
#include "portmidioutputnode.h"

PortMidiPlugin	*PortMidiPlugin::mInstance = nullptr;

QList<QUuid>	NodeControlBase::PID_UUID;

ClassEntry	NodeClasses[] =
{
	ClassEntry( "MIDI Input",			"PortMidi", ClassEntry::None, NID_PORTMIDI_INPUT,  &PortMidiInputNode::staticMetaObject ),
	ClassEntry( "MIDI Output",			"PortMidi", ClassEntry::None, NID_PORTMIDI_OUTPUT, &PortMidiOutputNode::staticMetaObject ),
	ClassEntry()
};

ClassEntry PinClasses[] =
{
	ClassEntry()
};

PortMidiPlugin::PortMidiPlugin( void )
{
	mInstance = this;

	//-------------------------------------------------------------------------
	// Install translator

	static QTranslator		Translator;

	if( Translator.load( QLocale(), QLatin1String( "translations" ), QLatin1String( "_" ), ":/" ) )
	{
		qApp->installTranslator( &Translator );
	}
}

PluginInterface::InitResult PortMidiPlugin::initialise( fugio::GlobalInterface *pApp, bool pLastChance )
{
	Q_UNUSED( pLastChance )

	mApp = pApp;

	if( !DeviceMidi::deviceInitialise() )
	{
		return( INIT_FAILED );
	}

	mApp->registerNodeClasses( NodeClasses );

	mApp->registerPinClasses( PinClasses );

	connect( mApp->qobject(), SIGNAL(frameInitialise(qint64)), this, SLOT(onGlobalFrameInitialise(qint64)) );
	connect( mApp->qobject(), SIGNAL(frameEnd(qint64)), this, SLOT(onGlobalFrameEnd(qint64)) );

	return( INIT_OK );
}

void PortMidiPlugin::deinitialise( void )
{
	mApp->unregisterPinClasses( PinClasses );

	mApp->unregisterNodeClasses( NodeClasses );

	DeviceMidi::deviceDeinitialise();

	mApp = 0;
}

void PortMidiPlugin::onGlobalFrameInitialise(qint64)
{
	DeviceMidi::devicePacketStart();
}

void PortMidiPlugin::onGlobalFrameEnd( qint64 )
{
	DeviceMidi::devicePacketEnd();
}
