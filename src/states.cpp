#include "states.h"
#include "defines.h"

namespace States {

	/*** states definitions ***/
	Base menu(pgmstr_emptystr);
	Confirm confirm(pgmstr_finished, pgmstr_press2continue);
	Confirm heater_error(pgmstr_heater_error, pgmstr_please_restart);
	Timer washing(pgmstr_washing, config.fans_washing_speed, &config.washing_run_time, &confirm);
	// FIXME - would be better to set PI regulator and manage heater for drying/curing?
	TimerHeater drying(pgmstr_drying, config.fans_drying_speed, &config.drying_run_time, &confirm);
	Curing curing(pgmstr_curing, config.fans_curing_speed, &config.curing_run_time, &confirm);
	TimerHeater drying_curing(pgmstr_drying, config.fans_drying_speed, &config.drying_run_time, &curing);
	TimerHeater resin(pgmstr_heating, config.fans_menu_speed, &config.resin_preheat_run_time, &confirm, &config.resin_target_temp);
	uint8_t max_warmup_run_time = MAX_WARMUP_RUN_TIME;
	Warmup warmup_print(pgmstr_warmup, &max_warmup_run_time, nullptr, &config.target_temp);
	Warmup warmup_resin(pgmstr_warmup, &max_warmup_run_time, &resin, &config.resin_target_temp);

	uint8_t heater_selftest_time = 10;
	TimerHeater selftest_heater(pgmstr_heater_test, config.fans_menu_speed, &heater_selftest_time, &confirm);
	uint8_t led_selftest_time = 10;
	Curing selftest_led(pgmstr_led_test, config.fans_menu_speed, &led_selftest_time, &selftest_heater);

	Base selftest_rotation(pgmstr_rotation_test);
	Base selftest_fans(pgmstr_fans_test);
	Base selftest_tank(pgmstr_ipatank_test);
	Base selftest_cover(pgmstr_cover_test);


	/*** states data ***/
	Base* active_state = &menu;

	void init() {
		active_state->start();
	}

	void loop(Events& events) {
		Base* new_state = active_state->process_events(events);
		if (!new_state) {
			new_state = active_state->loop();
		}
		if (new_state) {
			change(new_state);
		}
	}

	void change(Base* new_state) {
		active_state->stop();
		active_state = new_state;
		active_state->start();
	}

}
