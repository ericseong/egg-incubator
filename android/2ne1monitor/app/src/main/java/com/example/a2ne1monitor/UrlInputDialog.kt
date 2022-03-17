package com.example.a2ne1monitor

import android.app.AlertDialog
import android.content.Context
import android.util.Log
import android.view.LayoutInflater
import android.widget.EditText
import android.widget.Toast

class UrlInputDialog( context: Context, ssWebView: SessionStatWebView ) : androidx.lifecycle.ViewModel()  {
    val context = context
    val ssWebView = ssWebView
    val urlStorage = UrlStorage(context)

    private fun urlSetRequest( url: String ): Boolean {

        val ret = makeUrlSetRequest(url)
        when( ret ) {
            true -> Log.d("2NE1", "makeUrlSetRequest() with url " + url + " failed.")
        }

        return ret
    }

    private fun urlGetRequest(): String? {
        val url = makeUrlGetRequest()

        when( url ) {
            null -> Log.d("2NE1", "makeUrlGetRequest() " + "failed.")
        }
        Log.d("2NE1", "urlGetRequest(): retrieved url is "+"\""+url+"\"")

        return url
    }

    fun showMessageBox() {
        //Inflate the dialog as custom view
        val layoutInflator = LayoutInflater.from( context )
        val messageBoxView = layoutInflator.inflate(R.layout.dialog_url_input, null)
        val editTextUrl = messageBoxView.findViewById<EditText>(R.id.url_edit)

        var text: String? = urlGetRequest()
        Log.d( "2NE1", "showMessageBox(): Url is read from storage: " + text )

        editTextUrl.setText( text.toString() )

        //AlertDialogBuilder
        val messageBoxBuilder = AlertDialog.Builder( context ).setView(messageBoxView)

        // yes button
        messageBoxBuilder.setPositiveButton(R.string.dialog_ok,
            { dialog, id ->

                Log.d("2NE1", "Before loadUrl()")

                if( text != null ) {
                    //ssWebView.loadURL(editTextUrl.text.toString())
                    val url: String = editTextUrl.text.toString()
                    if( !url.equals(null) and !url.equals("") ) {
                        urlSetRequest( url )
                        ssWebView.loadURL( url )
                        try {
                            Toast.makeText( context,url + "에서 페이지를 다시 가져옵니다.", Toast.LENGTH_SHORT).show()
                        } finally {
                            //if (BuildConfig.DEBUG)
                            //    StrictMode.setThreadPolicy(oldPolicy)
                        }
                    }
                }

                Log.d("2NE1", "After loadUrl()")
                dialog.cancel()
            }
        )

        // url initialization button
        messageBoxBuilder.setNegativeButton(R.string.dialog_neutral,
            { dialog, id ->
                val initUrl: String = context.getString(R.string.site_to_load)
                editTextUrl.setText( initUrl )
                urlSetRequest( initUrl )
                ssWebView.loadURL( initUrl )
                dialog.cancel()
                try {
                    Toast.makeText( context, initUrl + "에서 페이지를 다시 가져옵니다.", Toast.LENGTH_SHORT).show()
                } finally {
                    //if (BuildConfig.DEBUG)
                    //    StrictMode.setThreadPolicy(oldPolicy)
                }
                Log.d("2NE1", "Url input initialized to the initial setting.")
            }
        )

        // cancel button
        messageBoxBuilder.setNeutralButton(R.string.dialog_cancel,
            { dialog, id ->
                dialog.cancel()
                Log.d("2NE1", "Url input cancelled.")
            }
        )

        // show dialog
        messageBoxBuilder.show()
    }

    private fun makeUrlSetRequest(url: String) : Boolean {
        return urlStorage.setUrlToStorage( url )
    }

    private fun makeUrlGetRequest() : String? {
        return urlStorage.getUrlFromStorage()
    }
}