#include <application.h>
#include <bc_servo.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

bc_servo_t servo;

bc_gfx_t *gfx;

int angle = 0;

void update_lcd(void)
{
    if (!bc_gfx_display_is_ready(gfx))
    {
        return;
    }

    bc_system_pll_enable();

    bc_gfx_clear(gfx);

    bc_gfx_set_font(gfx, &bc_font_ubuntu_24);

    bc_gfx_printf(gfx, 15, 15, 1, "%d", angle);

    bc_gfx_update(gfx);

    bc_system_pll_disable();
}

void update_angle(int increment)
{
    angle += increment;

    if (angle > 180)
    {
        angle = 180;
    }
    else if (angle < 0)
    {
        angle = 0;
    }

    bc_log_info("Angle %d", angle);

    bc_servo_set_angle(&servo, angle);

    update_lcd();
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    static int increment = 30;

    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_set_mode(&led, BC_LED_MODE_TOGGLE);

        if (angle % 30 != 0)
        {
            angle -= angle % 30;
        }

        if (angle >= 180)
        {
            increment = -30;
        } else if (angle <= 0) {
            increment = 30;
        }

        update_angle(increment);
    }
}

void module_encoder_event_handler(bc_module_encoder_event_t event, void *event_param)
{
    if (event == BC_MODULE_ENCODER_EVENT_ROTATION)
    {
        update_angle(bc_module_encoder_get_increment());
    }
}

void application_init(void)
{
    bc_log_init(BC_LOG_LEVEL_DEBUG, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize LCD module
    bc_module_lcd_init();
    gfx = bc_module_lcd_get_gfx();
    update_lcd();

    if (bc_module_encoder_is_present())
    {
        bc_module_encoder_init();
        bc_module_encoder_set_event_handler(module_encoder_event_handler, NULL);
    }

    bc_servo_init(&servo, BC_PWM_P1);
    bc_servo_set_microseconds(&servo, 1500);
    bc_servo_set_angle(&servo, angle);

    bc_log_info("bc_servo_set_microseconds 1500us");
}

void application_task(void)
{
    // update_lcd();
}
