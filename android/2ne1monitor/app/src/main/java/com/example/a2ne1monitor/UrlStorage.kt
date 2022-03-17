package com.example.a2ne1monitor

import android.content.Context
import android.util.Log
import java.io.File

class UrlStorage( context: Context ) {
    val context = context

    // set url to internal storage or hidden file
    fun setUrlToStorage(url: String) : Boolean {
        var ret = false
        val fileName: String = R.string.filename_site_to_load.toString()
        val file = File(context.filesDir.toString(), fileName)

        if (!file.exists()) {
            file.mkdirs() // create dirs along with all non-existing parents
            Log.d("2NE1", "File does not exist. Created a dir.")
        } else {
            Log.d("IST", "Dir for " + "\"" + fileName + "\"" + " already exists")
        }

        try {
            val myFile = File(file,fileName)
            myFile.writeText(url)
            Log.d("2NE1", "Wrote " + "\"" + url + "\"" + " to " + context.filesDir.toString()+"/"+fileName )
        } catch(e:java.lang.Exception) {
            e.printStackTrace()
            ret = true
        }

        return ret
    }

    // get url from internal storage or hidden file
    fun getUrlFromStorage() : String {

        var text = String()
        val fileName = R.string.filename_site_to_load.toString()
        val file = File(context.filesDir.toString(), fileName)
        if (!file.exists()) {
            setUrlToStorage(R.string.site_to_load.toString()) // create a file and stuff with string resource
        }
        try {
            val myFile = File(file,fileName)
            text = myFile.readText()
            Log.d("2NE1", "Read " + "\"" + text + "\"" + " from " + context.filesDir.toString()+"/"+fileName )
        } catch(e:java.lang.Exception) {
            e.printStackTrace()
        }

        return text
    }
}