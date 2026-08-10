package com.mechrobotix.aprendels

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import androidx.core.content.ContextCompat
import java.io.IOException
import java.io.OutputStream
import java.util.UUID

data class PairedBluetoothDevice(
    val name: String,
    val address: String,
    val device: BluetoothDevice
)

object Esp32BluetoothClient {

    private val sppUuid: UUID =
        UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")

    private val connectionLock = Any()

    @Volatile
    private var socket: BluetoothSocket? = null

    @Volatile
    private var outputStream: OutputStream? = null

    @Volatile
    var connectedName: String? = null
        private set

    val isConnected: Boolean
        get() = socket?.isConnected == true && outputStream != null

    fun hasBluetoothConnectPermission(context: Context): Boolean {
        return Build.VERSION.SDK_INT < Build.VERSION_CODES.S ||
            ContextCompat.checkSelfPermission(
                context,
                Manifest.permission.BLUETOOTH_CONNECT
            ) == PackageManager.PERMISSION_GRANTED
    }

    fun getBluetoothAdapter(context: Context): BluetoothAdapter? {
        val bluetoothManager = context.getSystemService(BluetoothManager::class.java)
        return bluetoothManager?.adapter
    }

    fun isBluetoothEnabled(context: Context): Boolean {
        return getBluetoothAdapter(context)?.isEnabled == true
    }

    fun listPairedDevices(context: Context): List<PairedBluetoothDevice> {
        if (!hasBluetoothConnectPermission(context)) {
            return emptyList()
        }

        val adapter = getBluetoothAdapter(context) ?: return emptyList()

        return adapter.bondedDevices
            .map { device ->
                PairedBluetoothDevice(
                    name = device.name ?: "Dispositivo sin nombre",
                    address = device.address,
                    device = device
                )
            }
            .sortedWith(
                compareByDescending<PairedBluetoothDevice> {
                    it.name.equals("ESP32_Mano", ignoreCase = true)
                }.thenBy { it.name.lowercase() }
            )
    }

    @Throws(IOException::class, SecurityException::class)
    fun connect(context: Context, pairedDevice: PairedBluetoothDevice) {
        if (!hasBluetoothConnectPermission(context)) {
            throw SecurityException("Falta el permiso BLUETOOTH_CONNECT.")
        }

        disconnect()

        val newSocket = pairedDevice.device.createRfcommSocketToServiceRecord(sppUuid)

        try {
            newSocket.connect()
            val newOutputStream = newSocket.outputStream

            synchronized(connectionLock) {
                socket = newSocket
                outputStream = newOutputStream
                connectedName = pairedDevice.name
            }
        } catch (exception: Exception) {
            try {
                newSocket.close()
            } catch (_: Exception) {
            }
            throw exception
        }
    }

    @Throws(IOException::class)
    fun sendCommand(command: Char) {
        val normalizedCommand = command.uppercaseChar()

        if (normalizedCommand !in 'A'..'Z' && normalizedCommand != '0') {
            throw IOException("Comando no válido.")
        }

        synchronized(connectionLock) {
            val activeSocket = socket
            val output = outputStream

            if (activeSocket?.isConnected != true || output == null) {
                throw IOException("No hay conexión activa.")
            }

            try {
                output.write(byteArrayOf(normalizedCommand.code.toByte(), '\n'.code.toByte()))
                output.flush()
            } catch (exception: IOException) {
                disconnect()
                throw exception
            }
        }
    }

    fun disconnect() {
        synchronized(connectionLock) {
            try {
                outputStream?.close()
            } catch (_: Exception) {
            }

            try {
                socket?.close()
            } catch (_: Exception) {
            }

            outputStream = null
            socket = null
            connectedName = null
        }
    }
}
