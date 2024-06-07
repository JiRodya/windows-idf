idf_component_register(
    SRCS "src/tempSensor.c"
    INCLUDE_DIRS "inc"
    REQUIRES driver onewire_bus # Add this line to specify the dependency
)
