// stdafx.h : Fichier Include pour les fichiers Include systme standard,
// ou les fichiers Include spcifiques aux projets qui sont utiliss frquemment,
// et sont rarement modifis

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclure les en-ttes Windows rarement utiliss
#endif

// Modifiez les dfinitions suivantes si vous devez cibler une plate-forme avant celles spcifies ci-dessous.
// Reportez-vous  MSDN pour obtenir les dernires informations sur les valeurs correspondantes pour les diffrentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalits spcifiques  Windows95 et Windows NT4 ou version ultrieure.
#define WINVER 0x0501		// Attribuez la valeur approprie  cet lment pour cibler Windows98 et Windows2000 ou version ultrieure.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalits spcifiques  Windows NT4 ou version ultrieure.
#define _WIN32_WINNT 0x0501		// Attribuez la valeur approprie  cet lment pour cibler Windows98 et Windows2000 ou version ultrieure.
#endif

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalits spcifiques  Windows98 ou version ultrieure.
#define _WIN32_WINDOWS 0x0501 // Attribuez la valeur approprie  cet lment pour cibler WindowsMe ou version ultrieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalits spcifiques  IE4.0 ou version ultrieure.
#define _WIN32_IE 0x0501	// Attribuez la valeur approprie  cet lment pour cibler IE5.0 ou version ultrieure.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// certains constructeurs CString seront explicites

// This prevents a lot of silly warnings regarding the use of the STL
#pragma warning (disable: 4786)
// This tells BCAM to use the MFC
#define USE_MFC
// BCAM can be used with Windows >= Win2k only
//srybex #include <BcamUtility.h>

// d�sactive le masquage MFC de certains messages d'avertissement courants et par ailleurs souvent ignors
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // composants MFC principaux et standard
#include <afxext.h>         // extensions MFC
#include <afxdisp.h>        // Classes MFC Automation
#include <afxtempl.h>		// utile pour les template comme CList

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Prise en charge des MFC pour les contrles communs Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX Measurement Studio
#include "NiIncludes.h"
//}}AFX Measurement Studio
