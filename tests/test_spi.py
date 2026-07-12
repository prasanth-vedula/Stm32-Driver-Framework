from pathlib import Path

def test_spi_files_exist():
    assert Path("Drivers/Inc/spi.h").exists()
    assert Path("Drivers/Src/spi.c").exists()

def test_spi_header_contains_api():
    text = Path("Drivers/Inc/spi.h").read_text(encoding="utf-8")
    assert "SPI_Init" in text
    assert "SPI_Transmit" in text
    assert "SPI_Receive" in text
    assert "SPI_TransmitReceive" in text