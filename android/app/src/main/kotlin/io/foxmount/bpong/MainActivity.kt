package io.foxmount.bpong

import android.util.Log
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import java.io.PrintWriter
import java.io.StringWriter
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class MainActivity : FlutterActivity() {
    
    enum class PARTS{
        TRIANGLE_R,
        TRIANGLE_B,
        SHAPE_R,
        SHAPE_B
    }
    
    private val CHANNEL = "io.foxmount.bpong/udp"

    var r1 = "00"
    var g1 = "00"
    var b1 = "00"
    var r2 = "00"
    var g2 = "00"
    var b2 = "00"
    var r3 = "00"
    var g3 = "00"
    var b3 = "00"
    var r4 = "00"
    var g4 = "00"
    var b4 = "00"

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)
        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, CHANNEL).setMethodCallHandler { call, result ->
            if (call.method == "sendUdpColor") {
                val batteryLevel:Int = sendUdpColor(call)
                if (batteryLevel != -1) {
                    result.success(batteryLevel)
                } else {
                    result.error("UNAVAILABLE", "Battery level not available.", null)
                }
//            } else if (call.method == "getWifiListAsString") {
//                val checker = WiFiChecker(context) { success, scanResults ->
//                    Log.d("WiFiChecker", "Result = " + scanResults!!.size)
//                    if (success) {
//                        val wifiList = scanResults.mapNotNull { it!!.SSID to it.level }.joinToString(separator = ";\n")
//                        result.success(wifiList)
//                    } else {
//                        result.error("UNAVAILABLE", "Wifi List not available.", null)
//                    }
//                }
            } else {
                result.notImplemented()
            }
        }

    }

    private fun sendUdpColor(call: MethodCall): Int {
        val ip = call.argument<String>("ip")?:"192.168.31.249" // hello world
        val red = call.argument<Int>("red")?:0 // hello world
        val green = call.argument<Int>("green")?:0 // hello world
        val blue = call.argument<Int>("blue")?:0 // hello world
        val partOfColors = call.argument<Int>("partOfColors")?:0 // hello world
val part = PARTS.values()[partOfColors]
        val message = collectData(part,red,green,blue)
        val thread = Thread {
            try {
                sendToServer(ip, message + System.currentTimeMillis())

            } catch (e: java.lang.Exception) {
                val errors = StringWriter()
                e.printStackTrace(PrintWriter(errors))
                val hier2: String = errors.toString()
            } catch (th: Throwable) {
                val errors = StringWriter()
                th.printStackTrace(PrintWriter(errors))
                val hier3: String = errors.toString()
            }
        }

        thread.start()
        return 1

    }

    // Method to send Sting to UDP Server
    fun sendToServer(ip: String, msg: String) {
        val clientSocket = DatagramSocket()
        val ipaddr: InetAddress = InetAddress.getByName(ip)
        var sendData = ByteArray(1024)
        val receiveData = ByteArray(1024)
        sendData = msg.toByteArray()
        val sendPacket = DatagramPacket(sendData, sendData.size, ipaddr, 8888)
        clientSocket.send(sendPacket)
        clientSocket.close()
    }

    fun collectData(partOfColors: PARTS, red: Int, green: Int, blue: Int): String {
        var result = ""

        when (partOfColors) {
            PARTS.TRIANGLE_R -> {
                r1 = red.toString(16)
                g1 = green.toString(16)
                b1 = blue.toString(16)

            }
            PARTS.TRIANGLE_B -> {
                r2 = red.toString(16)
                g2 = green.toString(16)
                b2 = blue.toString(16)

            }
            PARTS.SHAPE_R -> {
                r3 = red.toString(16)
                g3 = green.toString(16)
                b3 = blue.toString(16)

            }
            PARTS.SHAPE_B -> {
                r4 = red.toString(16)
                g4 = green.toString(16)
                b4 = blue.toString(16)

            }
        }
       
        if (r1.length < 2) r1 = '0' + r1
        if (g1.length < 2) g1 = '0' + g1
        if (b1.length < 2) b1 = '0' + b1
        if (r2.length < 2) r2 = '0' + r2
        if (g2.length < 2) g2 = '0' + g2
        if (b2.length < 2) b2 = '0' + b2
        if (r3.length < 2) r3 = '0' + r3
        if (g3.length < 2) g3 = '0' + g3
        if (b3.length < 2) b3 = '0' + b3
        if (r4.length < 2) r4 = '0' + r4
        if (g4.length < 2) g4 = '0' + g4
        if (b4.length < 2) b4 = '0' + b4
        var rgb1 = 'a' + r1 + g1 + b1
        var rgb2 = 'b' + r2 + g2 + b2
        var rgb3 = 'c' + r3 + g3 + b3
        var rgb4 = 'd' + r4 + g4 + b4

        var rgb = rgb1 + rgb2 + rgb3 + rgb4 + 'o'
        Log.d("RGB: ", rgb)
        return rgb

    }


}
