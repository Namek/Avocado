package info.czekanski.avocado

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.widget.ScrollView
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)
        supportActionBar?.setIcon(R.mipmap.ic_launcher)

        Avocado.init()
        Avocado.loadBios(assets.open("SCPH1001.BIN").readBytes(512 * 1024))
        update()

        emulate_frame.setOnClickListener {
            for (i in 1..60) {
                Avocado.emulateFrame()
                update()
            }
        }
    }

    fun update() {
        sample_text.text = String.format("PC: 0x%x", Avocado.getPc())
        val logs = Avocado.getLogs()
        Log.d("Avocado", logs)
        text_logs.append(logs)
        scroll_view.fullScroll(ScrollView.FOCUS_DOWN)
    }
}
