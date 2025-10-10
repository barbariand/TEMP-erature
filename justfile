
set positional-arguments
export GUM_CONFIRM_PROMPT_FOREGROUND := "#a4bca6"
export GUM_CONFIRM_SELECTED_BACKGROUND := "#4b7c7b"
export GUM_CONFIRM_UNSELECTED_BACKGROUND := "#2c4d4e"

default:
  pio run

upload:
  if [! -e /dev/ttyACM0 ]; then echo "/dev/ttyACM0 does not exist"; fi
  pio run --target upload --upload-port /dev/ttyACM0
