idf_component_register(SRCS
    "HEP-1000-X.cpp"
    "PMBus_ABIO.cpp"
    "PMBus_Support.cpp"
    INCLUDE_DIRS "."
    REQUIRES Adafruit_BusIO arduino-esp32
)
