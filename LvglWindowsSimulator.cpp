#include <Windows.h>

#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

static lv_style_t style_btn, style_button_pressed, style_screen;
lv_obj_t* list1;
lv_obj_t* btn[9];
lv_obj_t* btn1, * btn2, * btn3, * btn4;
lv_obj_t* label1, * label2;
lv_obj_t* btnx = NULL;
lv_timer_t* btn_timer;
int8_t menu_select;
int8_t change_state;
bool click_long = false;
bool disable = true;
bool up = false;
bool bot = false;
bool back = false;
bool enter = false;
uint8_t state;

lv_indev_t* pointer_indev;
lv_indev_t* keypad_indev;
lv_indev_t* encoder_indev;

enum buttons {
    BT_CANCEL = 0,
    BT_PRESSED,
};

// Função para mudar o estado do botão
void state_btn(uint8_t state) {
    switch (state) {
    case BT_CANCEL:
        lv_obj_remove_style(btnx, &style_button_pressed, 0);
        lv_obj_add_style(btnx, &style_btn, 0);
        break;
    case BT_PRESSED:
        lv_obj_remove_style(btnx, &style_btn, 0);
        lv_obj_add_style(btnx, &style_button_pressed, 0);
        break;
    }
}

// Função que será chamada quando o timer expirar
void btn_timer_callback(lv_timer_t* timer) {
    state_btn(BT_CANCEL);
}

// Função de callback para o evento de retorno do botão
void btn_event_handler(lv_event_t* e) {
    lv_obj_t* btn_rcv = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_clear_state(btn[menu_select], LV_STATE_PRESSED | LV_STATE_CHECKED);
    btnx = btn_rcv;
    state_btn(BT_PRESSED);
    if (btn_rcv == btn1) {
        menu_select = (menu_select + 1 >= 9) ? 1 : menu_select + 1;
        bot = false;
        lv_obj_scroll_to_y(list1, (menu_select >= 5) ? 110 : 0, LV_ANIM_OFF);
    }
    if (btn_rcv == btn2) {
        menu_select = (menu_select - 1 <= 0) ? 8 : menu_select - 1;
        up = false;

        lv_obj_scroll_to_y(list1, (menu_select <= 4) ? 0 : 110, LV_ANIM_OFF);
    }
    lv_obj_add_state(btn[menu_select], LV_STATE_PRESSED | LV_STATE_CHECKED);

    if (btn_timer) {
        lv_timer_del(btn_timer);
    }

    btn_timer = lv_timer_create(btn_timer_callback, 500, NULL);
}

static void style_init(void) {
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_bg_color(&style_btn, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_bg_grad_color(&style_btn, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_grad_dir(&style_btn, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_btn, lv_color_black());
    lv_style_set_border_opa(&style_btn, LV_OPA_20);
    lv_style_set_border_width(&style_btn, 2);

    lv_style_set_text_color(&style_btn, lv_color_black());

    lv_style_init(&style_button_pressed);
    lv_style_set_radius(&style_button_pressed, 10);
    lv_style_set_bg_opa(&style_button_pressed, LV_OPA_COVER);
    lv_style_set_bg_color(&style_button_pressed, lv_color_hex(0x8b9d9f));
    lv_style_set_bg_grad_color(&style_button_pressed, lv_color_hex(0xe7ebeb));
    lv_style_set_bg_grad_dir(&style_button_pressed, LV_GRAD_DIR_VER);

    lv_style_set_border_color(&style_button_pressed, lv_color_black());
    lv_style_set_border_opa(&style_button_pressed, LV_OPA_20);
    lv_style_set_border_width(&style_button_pressed, 2);

    lv_style_set_text_color(&style_button_pressed, lv_color_black());
}

void btn_obj(void) {
    style_init();

    if (btn1 != NULL) {
        lv_obj_del(btn1);
        btn1 = NULL;
    }
    btn1 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn1);
    lv_obj_set_pos(btn1, 12, 210);
    lv_obj_set_size(btn1, 70, 30);
    lv_obj_add_style(btn1, &style_btn, 0);
    lv_obj_set_style_bg_img_src(btn1, LV_SYMBOL_DOWN, 0);
    lv_obj_add_event_cb(btn1, btn_event_handler, LV_EVENT_CLICKED, NULL);

    if (btn2 != NULL) {
        lv_obj_del(btn2);
        btn2 = NULL;
    }
    btn2 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn2);
    lv_obj_set_pos(btn2, 87, 210);
    lv_obj_set_size(btn2, 70, 30);
    lv_obj_add_style(btn2, &style_btn, 0);
    lv_obj_set_style_bg_img_src(btn2, LV_SYMBOL_UP, 0);
    lv_obj_add_event_cb(btn2, btn_event_handler, LV_EVENT_CLICKED, NULL);

    if (btn3 != NULL) {
        lv_obj_del(btn3);
        btn3 = NULL;
    }
    btn3 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn3);
    lv_obj_set_pos(btn3, 162, 210);
    lv_obj_set_size(btn3, 70, 30);
    lv_obj_add_style(btn3, &style_btn, 0);
    label1 = lv_label_create(btn3);
    lv_label_set_text(label1, "BACK");
    lv_obj_center(label1);
    lv_obj_add_event_cb(btn3, btn_event_handler, LV_EVENT_CLICKED, NULL);

    if (btn4 != NULL) {
        lv_obj_del(btn4);
        btn4 = NULL;
    }
    btn4 = lv_btn_create(lv_scr_act());
    lv_obj_remove_style_all(btn4);
    lv_obj_set_pos(btn4, 237, 210);
    lv_obj_set_size(btn4, 70, 30);
    lv_obj_add_style(btn4, &style_btn, 0);
    label2 = lv_label_create(btn4);
    lv_label_set_text(label2, "ENTER");
    lv_obj_center(label2);
    lv_obj_add_event_cb(btn4, btn_event_handler, LV_EVENT_CLICKED, NULL);
}


void lv_list(void) {
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, lv_palette_main(LV_PALETTE_GREY));
    lv_obj_add_style(lv_scr_act(), &style_screen, LV_STATE_DEFAULT);

    list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1, 160, 180);
    lv_obj_center(list1);
    lv_obj_align(list1, LV_ALIGN_CENTER, 0, -15);

    lv_list_add_text(list1, "Menu");

    btn[1] = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    btn[2] = lv_list_add_btn(list1, LV_SYMBOL_WIFI, "WiFi");
    btn[3] = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "Arquivo");
    btn[4] = lv_list_add_btn(list1, LV_SYMBOL_PLUS, "Adicionar");
    btn[5] = lv_list_add_btn(list1, LV_SYMBOL_LIST, "Lista");
    btn[6] = lv_list_add_btn(list1, LV_SYMBOL_EDIT, "Editar");
    btn[7] = lv_list_add_btn(list1, LV_SYMBOL_BATTERY_FULL, "Bateria");
    btn[8] = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Voltar");
    menu_select = 1;

    lv_obj_add_state(btn[menu_select], LV_STATE_PRESSED | LV_STATE_CHECKED);

    btn_obj();
}

int main()
{
    lv_init();

    /*
     * Optional workaround for users who wants UTF-8 console output.
     * If you don't want that behavior can comment them out.
     *
     * Suggested by jinsc123654.
     */
#if LV_TXT_ENC == LV_TXT_ENC_UTF8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    int32_t zoom_level = 100;
    bool allow_dpi_override = false;
    bool simulator_mode = true;
    lv_display_t* display = lv_windows_create_display(
        L"LVGL Windows Simulator Display 1",
        320,//800,
        240,//480,
        zoom_level,
        allow_dpi_override,
        simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display);
    if (!window_handle)
    {
        return -1;
    }

    HICON icon_handle = LoadIconW(
        GetModuleHandleW(NULL),
        MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        SendMessageW(
            window_handle,
            WM_SETICON,
            TRUE,
            (LPARAM)icon_handle);
        SendMessageW(
            window_handle,
            WM_SETICON,
            FALSE,
            (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev)
    {
        return -1;
    }

    lv_indev_t* encoder_indev = lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev)
    {
        return -1;
    }

    //lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_music();
    lv_list();

    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}
