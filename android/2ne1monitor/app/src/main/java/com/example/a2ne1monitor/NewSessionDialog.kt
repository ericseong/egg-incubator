package com.example.a2ne1monitor

import android.app.AlertDialog
import android.content.Context
import android.util.Log
import android.view.LayoutInflater
import android.widget.EditText
import android.widget.Toast
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.net.HttpURLConnection

class NewSessionDialog( context: Context ) : androidx.lifecycle.ViewModel() {
    val context = context

    fun showMessageBox() {
        //Inflate the dialog as custom view
        val layoutInflator = LayoutInflater.from( context )
        val messageBoxView = layoutInflator.inflate(R.layout.dialog_new_session, null)
        val editTextPasswd = messageBoxView.findViewById<EditText>(R.id.new_session_passwd_edit)

        //AlertDialogBuilder
        val messageBoxBuilder = AlertDialog.Builder( context ).setView(messageBoxView)

        // ok button
        messageBoxBuilder.setPositiveButton(R.string.dialog_ok,
            { dialog, id ->
                val passwd = editTextPasswd.text.toString()
                if( passwd.equals("4737") ) {
                    val jsonBody = "{ \"new_session\": true }"
                    this.processNewSessionRequest(jsonBody)
                } else {
                    try {
                        Toast.makeText( context,"패스워드가 틀렸습니다.", Toast.LENGTH_SHORT).show()
                    } finally {
                        //if (BuildConfig.DEBUG)
                        //    StrictMode.setThreadPolicy(oldPolicy)
                    }
                }
                dialog.cancel()
            }
        )

        // cancel button
        messageBoxBuilder.setNeutralButton(R.string.dialog_cancel,
            { dialog, id ->
                dialog.cancel()
                Log.d("2NE1", "New session request cancelled.")
            }
        )
        // show dialog
        messageBoxBuilder.show()
    }

    private fun processNewSessionRequest( msg: String) {
        viewModelScope.launch(Dispatchers.IO) {
            val repository = RequestRepository()
            val result = repository.makePostRequest( UrlStorage(context).getUrlFromStorage(), msg )
            when (result.responseCode) {
                HttpURLConnection.HTTP_OK -> {
                    Log.d("2NE1", "New session request succeeded.")
                    notifyUINewSessionRequestResult(true)
                }
                else -> {
                    Log.d("2NE1", "New session request failed.")
                    notifyUINewSessionRequestResult(false)
                }
            }
        }
    }

    private fun notifyUINewSessionRequestResult(isSucc: Boolean) {
        android.os.Handler(android.os.Looper.getMainLooper()).post { // UI can only be drawn from main thread
            val text = arrayOf(
                "새로운 세션을 시작하였습니다.",
                "새로운 세션을 시작할 수 없습니다."
            )
            //val oldPolicy = StrictMode.allowThreadDiskReads()
            try {
                Toast.makeText(
                    context,
                    when(isSucc) {
                        true -> text[0]
                        else -> text[1]
                    },
                    Toast.LENGTH_LONG
                ).show()
            } finally {
                //if (BuildConfig.DEBUG)
                //    StrictMode.setThreadPolicy(oldPolicy)
            }
        } // post{}
    } // notifyUINewSessionRequestResult()
}