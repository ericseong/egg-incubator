package com.example.a2ne1monitor

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.webkit.WebChromeClient
import android.webkit.WebView
import android.webkit.WebViewClient

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val webView = findViewById<WebView>(R.id.webView)
        webView.settings.apply {
            javaScriptEnabled = true
            domStorageEnabled = true
            javaScriptCanOpenWindowsAutomatically = true
            mediaPlaybackRequiresUserGesture = false // hmm.. it does not work! so, I played a trick using hidden div and js injection.
            setSupportMultipleWindows(true)
        }
        webView.getSettings().setMediaPlaybackRequiresUserGesture(false)
        webView.webViewClient = object : WebViewClient() {
            override fun onPageFinished(view: WebView, url: String) {
                // page loading finished. do we have ways to get audiocontext?
                webView.loadUrl("javascript:(function() { document.getElementById('hidden_button').click(); })()");
            }
        }
        webView.loadUrl("http://192.168.219.119:12913")
    }
}
