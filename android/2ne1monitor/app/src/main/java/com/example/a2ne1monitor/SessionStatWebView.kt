package com.example.a2ne1monitor

import android.app.Activity
import android.util.Log
import android.webkit.WebResourceError
import android.webkit.WebResourceRequest
import android.webkit.WebView
import android.webkit.WebViewClient
import com.example.a2ne1monitor.databinding.ActivityMainBinding

class SessionStatWebView( binding: ActivityMainBinding ) : Activity() {

    private val webView = binding.webView
    private var isWebViewClientInitialized: Boolean = false
    private var currentUrl: String = ""

    fun loadURL( url: String ): WebView
    {
        if( !isWebViewClientInitialized ) {
            // in order not to create multiple instances of webviewclient
            isWebViewClientInitialized = true

            // 1. configure web view
            webView.settings.apply {
                javaScriptEnabled = true
                domStorageEnabled = true
                javaScriptCanOpenWindowsAutomatically = true
                mediaPlaybackRequiresUserGesture = false // hmm.. it does not work! so, I played a trick using hidden div and js injection.
                setSupportMultipleWindows(true)
                Log.d("2NE1", webView.settings.userAgentString)
            }

            // 2. create web view
            webView.webViewClient = object : WebViewClient() {
                override fun onPageStarted(
                    view: WebView,
                    url: String,
                    favicon: android.graphics.Bitmap?
                ) {
                    super.onPageStarted(view, url, favicon)
                    Log.d("2NE1", "OnPageStarted()..")
                }

                override fun onPageFinished(view: WebView, url: String) {
                    super.onPageFinished(view, url)
                    // page loading finished. the injection below is to avoid prohibited autoplay by triggering a simple user interaction to a hidden button.
                    webView.loadUrl("javascript:(function() { document.getElementById('hidden_button').click(); })()")
                    Log.d("2NE1", "OnPageFinished()..")
                }

                override fun onReceivedError(
                    view: WebView?,
                    request: WebResourceRequest?,
                    error: WebResourceError?
                ) {
                    super.onReceivedError(view, request, error)
                    // view?.reload()
                    view?.loadUrl( currentUrl )
                    Log.d("2NE1", "onReceivedError().. reload()..")
                }
                //override fun shouldOverrideUrlLoading(view: WebView?, url: String): Boolean {
                //    return false
                //}
            }
        }

        // 3. load a page onto a web view
        webView.loadUrl( url )
        currentUrl = url
        Log.d("2NE1", "loadUrl() with " + url )

        return webView
    }
}