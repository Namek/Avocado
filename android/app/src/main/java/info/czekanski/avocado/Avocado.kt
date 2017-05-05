package info.czekanski.avocado

object Avocado {
    init {
        System.loadLibrary("Avocado")
    }

    external fun init(): Boolean
    external fun loadBios(bios: ByteArray): Boolean
    external fun emulateFrame(): Boolean
    external fun getPc(): Int
    external fun getLogs(): String
}