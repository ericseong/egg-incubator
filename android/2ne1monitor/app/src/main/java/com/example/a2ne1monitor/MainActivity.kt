package com.example.a2ne1monitor

import android.app.AlertDialog
import android.os.Bundle
import android.os.StrictMode
import android.os.StrictMode.ThreadPolicy
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.webkit.*
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.net.HttpURLConnection

class MainActivity : AppCompatActivity() {

    // toolbar as a replacement of app bar
    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        val ret: Boolean = super.onCreateOptionsMenu(menu)
        menuInflater.inflate(R.menu.appbar_action, menu)
        Log.d( "2NE1", "onCreateOptionsMenu()..")

        return ret
    }

    override fun onPrepareOptionsMenu(menu: Menu?): Boolean {
        val ret: Boolean = super.onPrepareOptionsMenu(menu)
        return ret
    }

    // option menu handlers
    override fun onOptionsItemSelected(item: MenuItem) = when {
        item.itemId == R.id.action_new_session -> {
            //super.onOptionsItemSelected(item)
            handlerActionNewSession()
        }
        item.itemId == R.id.action_about -> {
            //super.onOptionsItemSelected(item)
           handlerActionAbout()
        }
        else -> {
            super.onOptionsItemSelected(item)
        }
    }

    private fun handlerActionNewSession(): Boolean {
        val dlg = NewSessionDialog()
        dlg.showMessageBox()
        Log.d("2NE1", "handlerActionNewSession()..")

        return true
    }

    private fun handlerActionAbout(): Boolean {
        val text = "2ne1 monitor v0.9"
        if (BuildConfig.DEBUG) {
            val oldPolicy = StrictMode.allowThreadDiskReads()
            try {
                Toast.makeText(this@MainActivity, text, android.widget.Toast.LENGTH_SHORT ).show()
            } finally {
                StrictMode.setThreadPolicy(oldPolicy)
            }
        }
        Log.d("2NE1", "In handlerActionAbout(), showed toast.")

        return true
    }

    // Main activity with a web view
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if(BuildConfig.DEBUG) {
            android.os.StrictMode.enableDefaults()
        }

        val tb: androidx.appcompat.widget.Toolbar = findViewById(R.id.app_toolbar)
        //tb.setTitle("2ne1 monitor") // see activity_main.xml
        setSupportActionBar(tb)

        // 1. config web view
        val webView = findViewById<WebView>(R.id.webView)
        webView.settings.apply {
            javaScriptEnabled = true
            domStorageEnabled = true
            javaScriptCanOpenWindowsAutomatically = true
            mediaPlaybackRequiresUserGesture = false // hmm.. it does not work! so, I played a trick using hidden div and js injection.
            setSupportMultipleWindows(true)
            Log.d( "2NE1", webView.settings.userAgentString )
        }

        // 2. create web view
        webView.webViewClient = object : WebViewClient() {
            override fun onPageStarted(view: WebView, url: String, favicon: android.graphics.Bitmap?) {
                super.onPageStarted(view, url, favicon)
                Log.d( "2NE1", "OnPageStarted()..")
            }
            override fun onPageFinished(view: WebView, url: String) {
                super.onPageFinished(view, url)
                // page loading finished. the injection below is to avoid prohibited autoplay by triggering a simple user interaction to a hidden button.
                webView.loadUrl("javascript:(function() { document.getElementById('hidden_button').click(); })()")
                Log.d( "2NE1", "OnPageFinished().." )
            }
            override fun onReceivedError(
                view: WebView?,
                request: WebResourceRequest?,
                error: WebResourceError?
            ) {
                super.onReceivedError(view, request, error)
                view?.reload()
                Log.d( "2NE1", "onReceivedError().. reload().." )
            }
        }

        // 3. load a page onto a web view
        webView.loadUrl( getString(R.string.site_to_load) )
        Log.d( "2NE1", "loadUrl() with " + getString(R.string.site_to_load) )

        return
    }

    // main activity
    override fun onResume() {
        val webView = findViewById<WebView>(R.id.webView)
        super.onResume()
        webView?.onResume()
        webView?.resumeTimers()
        Log.d( "2NE1", "onResume().." )
    }

    // main activity
    override fun onPause() {
        val webView = findViewById<WebView>(R.id.webView)
        super.onPause()
        webView?.onPause()
        webView?.pauseTimers()
        Log.d( "2NE1", "onPause().." )
    }

    private fun isNetworkConnected(): Boolean {
        //1
        val connectivityManager = getSystemService(android.content.Context.CONNECTIVITY_SERVICE) as android.net.ConnectivityManager
        //2
        val activeNetwork = connectivityManager.activeNetwork
        //3
        val networkCapabilities = connectivityManager.getNetworkCapabilities(activeNetwork)
        //4
        return networkCapabilities != null &&
                networkCapabilities.hasCapability(android.net.NetworkCapabilities.NET_CAPABILITY_INTERNET)
    }

    //    inner class MyRendererTrackingWebViewClient : WebViewClient() {
//        private var mWebView: WebView? = null
//        override fun onRenderProcessGone(view: WebView, detail: RenderProcessGoneDetail): Boolean {
//            if (!detail.didCrash()) {
//                Log.e(
//                    "MY_APP_TAG", ("System killed the WebView rendering process " +
//                            "to reclaim memory. Recreating...")
//                )
//                mWebView?.also { wv ->
//                    val webViewContainer: ViewGroup = findViewById<WebView>(R.id.webView)
//                    webViewContainer.removeView(wv)
//                    wv.destroy()
//                    mWebView = null
//                }
//                return true
//            }
//            Log.e("2NE1", "The WebView rendering process crashed!")
//            return false
//        }
//    }

    inner class NewSessionDialog : androidx.lifecycle.ViewModel() {

        fun showMessageBox() {
            //Inflate the dialog as custom view
            val messageBoxView = layoutInflater.inflate(R.layout.dialog_new_session, null)

            //AlertDialogBuilder
            val messageBoxBuilder = AlertDialog.Builder(this@MainActivity).setView(messageBoxView)

            // ok button
            messageBoxBuilder.setPositiveButton("CONFIRM",
                { dialog, id ->
                    val jsonBody = "{ \"new_session\": true }"
                        this.processNewSessionRequest(jsonBody)
                        dialog.cancel()
                }
            )

            // cancel button
            messageBoxBuilder.setNegativeButton("CANCEL",
                { dialog, id ->
                    dialog.cancel()
                    Log.d("2NE1", "New session request cancelled.")
                }
            )
            // show dialog
            messageBoxBuilder.show()
        }

        private fun processNewSessionRequest(msg: String) {
            viewModelScope.launch(Dispatchers.IO) {
                val repository = RequestRepository()
                val result = repository.makePostRequest(getString(R.string.site_to_load), msg)
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

        private fun notifyUINewSessionRequestResult( isSucc: Boolean ) {
            android.os.Handler(android.os.Looper.getMainLooper()).post { // UI can only be drawn from main thread
                if ( isSucc ) {
                    if (BuildConfig.DEBUG) {
                        val oldPolicy = StrictMode.allowThreadDiskReads()
                        try {
                            Toast.makeText(
                                this@MainActivity,
                                "New session request is returned with success.",
                                Toast.LENGTH_LONG
                            ).show()
                        } finally {
                            StrictMode.setThreadPolicy(oldPolicy)
                        }
                    }
                } else {
                    if (BuildConfig.DEBUG) {
                        val oldPolicy = StrictMode.allowThreadDiskReads()
                        try {
                            Toast.makeText(
                                this@MainActivity,
                                "New session request returned with failure.",
                                Toast.LENGTH_LONG
                            ).show()
                        } finally {
                            StrictMode.setThreadPolicy(oldPolicy)
                        }
                    }
                }
            } // post{}
        }
    } // end of inner class NewSessionDialog
}
