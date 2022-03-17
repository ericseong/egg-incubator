package com.example.a2ne1monitor

import android.os.Bundle
import android.os.StrictMode
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.webkit.*
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.a2ne1monitor.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    // toolbar as a replacement of app bar
    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        val ret: Boolean = super.onCreateOptionsMenu(menu)
        menuInflater.inflate(R.menu.appbar_action, menu)
        Log.d( "2NE1", "onCreateOptionsMenu()..")

        return ret
    }

    override fun onPrepareOptionsMenu(menu: Menu?): Boolean {
        return super.onPrepareOptionsMenu(menu)
    }

    // option menu handlers
    override fun onOptionsItemSelected(item: MenuItem) = when(item.itemId) {
        R.id.action_url -> {
            handlerActionUrlInput()
        }
        R.id.action_new_session -> {
            //super.onOptionsItemSelected(item)
            handlerActionNewSession()
        }
        R.id.action_about -> {
            //super.onOptionsItemSelected(item)
           handlerActionAbout()
        }
        else -> {
            super.onOptionsItemSelected(item)
        }
    }

    private fun handlerActionUrlInput(): Boolean {
        val dlg = UrlInputDialog( this, ssWebView )
        dlg.showMessageBox()
        Log.d("2NE1", "handlerActionUrlInput()..")

        return true
    }

    private fun handlerActionNewSession(): Boolean {
        val dlg = NewSessionDialog(this )
        dlg.showMessageBox()
        Log.d("2NE1", "handlerActionNewSession()..")

        return true
    }

    private fun handlerActionAbout(): Boolean {
        val text = BuildConfig.APPLICATION_ID + " " + BuildConfig.BUILD_TYPE + " " + BuildConfig.VERSION_NAME
        //val oldPolicy = StrictMode.allowThreadDiskReads()
        try {
            Toast.makeText(this@MainActivity, text, Toast.LENGTH_SHORT ).show()
        } finally {
            //if (BuildConfig.DEBUG)
            //    StrictMode.setThreadPolicy(oldPolicy)
        }
        Log.d("2NE1", "In handlerActionAbout(), showed toast.")

        return true
    }

    private lateinit var binding: ActivityMainBinding
    private lateinit var ssWebView: SessionStatWebView

    // Main activity with a web view
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root // to connect view to code
        setContentView(view)
        //setContentView(R.layout.activity_main)

        //if(BuildConfig.DEBUG)
        //    StrictMode.enableDefaults()

        val tb: androidx.appcompat.widget.Toolbar = binding.appToolbar
        //tb.setTitle("2ne1 monitor") // see activity_main.xml
        setSupportActionBar(tb)

        // webview
        ssWebView = SessionStatWebView( binding )

        // load url
        val url = UrlStorage(this).getUrlFromStorage()
        ssWebView.loadURL( url )

        return
    }

    // main activity
    override fun onResume() {
        val webView = binding.webView
        super.onResume()
        webView.onResume()
        webView.resumeTimers()
        Log.d( "2NE1", "onResume().." )
    }

    // main activity
    override fun onPause() {
        val webView = findViewById<WebView>(R.id.webView)
        super.onPause()
        webView.onPause()
        webView.pauseTimers()
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

}
