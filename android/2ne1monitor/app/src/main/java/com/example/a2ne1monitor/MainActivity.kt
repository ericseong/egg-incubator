package com.example.a2ne1monitor

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.view.ViewGroup
import android.webkit.*
import java.util.Timer
import kotlin.concurrent.schedule

class MainActivity : AppCompatActivity() {
    //        val webViewTracker_ = MyRendererTrackingWebViewClient()
    fun _config( wv: WebView ) {
        wv.settings.apply {
            javaScriptEnabled = true
            domStorageEnabled = true
            javaScriptCanOpenWindowsAutomatically = true
            mediaPlaybackRequiresUserGesture = false // hmm.. it does not work! so, I played a trick using hidden div and js injection.
            setSupportMultipleWindows(true)
            wv.getSettings().setMediaPlaybackRequiresUserGesture(false)
            Log.d( "2NE1", wv.getSettings().userAgentString )
        }
    }
    fun _create( wv: WebView ) {
        wv.webViewClient = object : WebViewClient() {
            override fun onPageFinished(view: WebView, url: String) {
                super.onPageFinished(view, url)
                // page loading finished. the injection below is to avoid prohibited autoplay by triggering a simple user interaction to a hidden button.
                wv.loadUrl("javascript:(function() { document.getElementById('hidden_button').click(); })()");
            }
            override fun onReceivedError(
                view: WebView?,
                request: WebResourceRequest?,
                error: WebResourceError?
            ) {
                super.onReceivedError(view, request, error)
                if(view != null)
                    view.reload()
            }
        }
    }
    fun _load(wv: WebView) {
        wv.loadUrl("http://192.168.219.119:12913")
        Log.d("2NE1", "loadUrl(\"http://192.168.219.119:12913\")" )
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val webView = findViewById<WebView>(R.id.webView)
        _config(webView)
        _create(webView)
        _load(webView)
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
    override fun onResume() {
        val webView = findViewById<WebView>(R.id.webView)
        super.onResume()
        webView.onResume()
        webView.resumeTimers()
    }
    override fun onPause() {
        val webView = findViewById<WebView>(R.id.webView)
        super.onPause()
        webView.onPause()
        webView.pauseTimers()
    }
}

