package com.rusefi.app;

import android.hardware.usb.UsbManager;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.dfu.DfuLogic;
import com.rusefi.io.ByteReader;
import com.rusefi.io.serial.AbstractIoStream;

import java.io.IOException;
import java.util.List;

public class AndroidSerial extends AbstractIoStream {
    private static final int ST_CDC = 0x5740;
    private final IncomingDataBuffer dataBuffer;

    private UsbSerialPort usbSerialPort;

    static List<UsbSerialDriver> findUsbSerial(UsbManager usbManager) {
        ProbeTable customTable = UsbSerialProber.getDefaultProbeTable();
        customTable.addProduct(DfuLogic.ST_VENDOR, ST_CDC, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);

        return prober.findAllDrivers(usbManager);
    }

    public AndroidSerial(UsbSerialPort usbSerialPort) {
        this.usbSerialPort = usbSerialPort;
        dataBuffer = IncomingDataBuffer.createDataBuffer("", this);
    }

    @Override
    public String getLoggingPrefix() {
        return "";
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    @Override
    public void setInputListener(DataListener listener) {
        ByteReader reader = buffer -> usbSerialPort.read(buffer, 5000);
        ByteReader.runReaderLoop("", listener, reader, this);
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        usbSerialPort.write(bytes, 1000);
    }
}
