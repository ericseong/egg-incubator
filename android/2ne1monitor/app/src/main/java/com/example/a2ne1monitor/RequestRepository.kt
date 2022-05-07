package com.example.a2ne1monitor

import android.net.TrafficStats
import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.net.HttpURLConnection
import java.net.URL

data class HttpResponse(
    var responseCode: Int,
    var result: String
)

class RequestRepository() {

    // http post request
    suspend fun makePostRequest( myUrl: String, body: String ) : HttpResponse {
        return withContext( Dispatchers.IO ) {
            httpRequest( "POST", myUrl, body )
        }
    }

    // http get request
    private fun httpRequest(method: String, myUrl: String, body: String ): HttpResponse {
        var inputStream: java.io.InputStream
        var outputStream: java.io.OutputStream
        var res = HttpResponse( -1, "" )

        val url = URL(myUrl)
        var conn: HttpURLConnection = url.openConnection() as HttpURLConnection
        conn.setRequestMethod(method) // "POST", "GET"
        conn.setConnectTimeout(1000) // one second
        conn.setReadTimeout(1000)
        conn.setRequestProperty("User-Agent", "Android/2ne1 monitor")
        conn.setRequestProperty("Accept", "text/html")
        conn.setRequestProperty("Accept-Language", "ko-KR")
        when(method) {
            "GET"-> {
                conn.setRequestProperty("Content-Type", "text/html") // The format of the content we're sending to the server
            }
            "POST" -> {
                conn.setRequestProperty("Content-Type", "application/json") // The format of the content we're sending to the server
            }
            else -> {
                Log.d("2NE1", "httpRequest(): Unknown method!")
            }
        }

        conn.doInput = true
        conn.doOutput = true

        // send / receive data
        //if (BuildConfig.DEBUG)
        //    TrafficStats.setThreadStatsTag(Thread.currentThread().id.toInt()) // to avoid strict mode violation
        try {
            conn.connect()

            // write
            outputStream = conn.outputStream
            outputStream.write( body.toByteArray() )
            outputStream.flush()

            // read
            inputStream = conn.inputStream
            res.responseCode = conn.getResponseCode()

            if (res.responseCode == HttpURLConnection.HTTP_OK) {
                res.result = inputStream.toString()
            }
            Log.d("2NE1", "Response is " + res.responseCode)
        } catch (e: Exception) {
            e.printStackTrace()
        }
        finally {
            conn.disconnect()
        }

        return res // will return null if something happened..
    }
}
