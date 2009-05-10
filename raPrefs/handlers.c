/*
** Handlers definition
**
** $Id: handlers.c,v 1.1 2008-09-02 23:24:26 abalabanb Exp $
*/

#include "handlers.h"

#include "gui_global.h"

#include <classes/window.h>

#include <gadgets/chooser.h>
#include <gadgets/getfile.h>
#include <gadgets/listbrowser.h>

#include <reaction/reaction_macros.h>

#include <proto/intuition.h>

extern struct Window *window;
extern struct Window *edit_brow_window;
extern struct Window *edit_mail_window;
extern struct Window *edit_ftp_window;

extern Object *win;
extern Object *edit_brow_win;
extern Object *edit_mail_win;
extern Object *edit_ftp_win;

STRPTR hidden_strings[] =
{
    "string 1",
    "string 2",
    "string 3",
    "string 4",
     NULL
};

BOOL HandleInput_Main_Win(void)
{
    uint32 result      = 0;
    uint16 code        = 0;
    BOOL   done        = FALSE;

    while ((result = RA_HandleInput(win, &code)))
    {
        switch(result & WMHI_CLASSMASK)
        {
            case WMHI_CLOSEWINDOW:
                done = TRUE;
                break;
            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    case OBJ_LBROWSER_BROW:
                    {
                        uint32 retval = 0;
                        IIntuition->GetAttr( LISTBROWSER_RelEvent, OBJ(OBJ_LBROWSER_BROW), &retval );
                        if (retval != LBRE_DOUBLECLICK) break;
                        printf("retval == LBRE_DOUBLECLICK\n");
                    }
                    case OBJ_EDIT_BROW:
                    {
                        struct URL_BrowserNode * bn = NULL;
                        IIntuition->GetAttr( LISTBROWSER_SelectedNode, OBJ(OBJ_LBROWSER_BROW), (ULONG*) &bn );
                        updateBrowserWindow( bn );
                        edit_brow_window = RA_OpenWindow(edit_brow_win);
                        break;
                    }
                    case OBJ_LBROWSER_MAIL:
                    {
                        uint32 retval = 0;
                        IIntuition->GetAttr( LISTBROWSER_RelEvent, OBJ(OBJ_LBROWSER_MAIL), &retval );
                        if (retval != LBRE_DOUBLECLICK) break;
                        printf("retval == LBRE_DOUBLECLICK\n");
                    }
                    case OBJ_EDIT_MAIL:
                    {
                        struct URL_MailerNode * mn = NULL;
                        IIntuition->GetAttr( LISTBROWSER_SelectedNode, OBJ(OBJ_LBROWSER_MAIL), (ULONG*) &mn );
                        updateMailerWindow( mn );
                        edit_mail_window = RA_OpenWindow(edit_mail_win);
                        break;
                    }
                    case OBJ_LBROWSER_FTP:
                    {
                        uint32 retval = 0;
                        IIntuition->GetAttr( LISTBROWSER_RelEvent, OBJ(OBJ_LBROWSER_FTP), &retval );
                        if (retval != LBRE_DOUBLECLICK) break;
                        printf("retval == LBRE_DOUBLECLICK\n");
                    }
                    case OBJ_EDIT_FTP:
                    {
                        struct URL_FTPNode * fn = NULL;
                        IIntuition->GetAttr( LISTBROWSER_SelectedNode, OBJ(OBJ_LBROWSER_FTP), (ULONG*) &fn );
                        updateFTPWindow( fn );
                        edit_ftp_window = RA_OpenWindow(edit_ftp_win);
                        break;
                    }
                    case OBJ_USE:
                    case OBJ_SAVE:
                    case OBJ_APPLY:
                        storePrefs((result & WMHI_GADGETMASK)==OBJ_SAVE);
                        break;
                    case OBJ_CANCEL:
                        done=TRUE;
                        break;
                }
                break;
            case WMHI_ICONIFY:
                if (RA_Iconify(win))
                {
                    window = NULL;
                    if (edit_brow_window)
                    {
                        RA_CloseWindow(edit_brow_win);
                        edit_brow_window = NULL;
                    }
                    if (edit_mail_window)
                    {
                        RA_CloseWindow(edit_mail_win);
                        edit_mail_window = NULL;
                    }
                    if (edit_ftp_window)
                    {
                        RA_CloseWindow(edit_ftp_win);
                        edit_ftp_window = NULL;
                    }
                }
                break;
            case WMHI_UNICONIFY:
                window = RA_OpenWindow(win);
                break;
        }
    }
    return (done);
}

void HandleInput_Edit_Brow_Win()
{
    uint32 result      = 0;
    uint16 code        = 0;

    while ((result = RA_HandleInput(edit_brow_win, &code)))
    {
        switch(result & WMHI_CLASSMASK)
        {
            case WMHI_CLOSEWINDOW:
                RA_CloseWindow(edit_brow_win);
                edit_brow_window = NULL;
                break;
            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    case OBJ_BROW_PATH_GET:
                        if (gfRequestFile(OBJ(OBJ_BROW_PATH_GET), edit_brow_window))
                        {
                        }
                        break;
                    case OBJ_BROW_PATH_CHOOSE:  // set Attrs according to the button clicked on.
                    case OBJ_BROW_AREXX_CHOOSE:
                    case OBJ_BROW_OPEN_CHOOSE:
                    case OBJ_BROW_NEW_CHOOSE:

                        IIntuition->SetAttrs( OBJ(OBJ_HIDDEN_CHOOSER),

                    //  NULL-terminated array of strings
                            CHOOSER_LabelArray, hidden_strings, TAG_DONE);

                    //  or an Exec List of labels (built from open ARexx ports ?)
                    //        CHOOSER_Labels, &chooserlist, TAG_DONE);

                        IIntuition->ActivateGadget(GAD(OBJ_HIDDEN_CHOOSER),
                                                   edit_brow_window, NULL);
                        break;
                }
        }
    }
}


void HandleInput_Edit_Mail_Win()
{
    uint32 result      = 0;
    uint16 code        = 0;

    while ((result = RA_HandleInput(edit_mail_win, &code)))
    {
        switch(result & WMHI_CLASSMASK)
        {
            case WMHI_CLOSEWINDOW:
                RA_CloseWindow(edit_mail_win);
                edit_mail_window = NULL;
                break;
            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    case OBJ_MAIL_PATH_GET:
                        if (gfRequestFile(OBJ(OBJ_MAIL_PATH_GET), edit_mail_window))
                        {
                        }
                        break;
                }
        }
    }
}


void HandleInput_Edit_FTP_Win()
{
    uint32 result      = 0;
    uint16 code        = 0;

    while ((result = RA_HandleInput(edit_ftp_win, &code)))
    {
        switch(result & WMHI_CLASSMASK)
        {
            case WMHI_CLOSEWINDOW:
                RA_CloseWindow(edit_ftp_win);
                edit_ftp_window = NULL;
                break;
            case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    case OBJ_FTP_PATH_GET:
                        if (gfRequestFile(OBJ(OBJ_FTP_PATH_GET), edit_ftp_window))
                        {
                        }
                        break;
                }
        }
    }
}
