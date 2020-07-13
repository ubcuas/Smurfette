use std::thread::sleep;
use std::time::{Duration, Instant};

pub struct TickTimer {
    lastcall: Instant,
    duration: Duration,
}

impl TickTimer {
    pub fn new(duration_ms: u64) -> TickTimer {
        return TickTimer {
            lastcall: Instant::now(),
            duration: Duration::from_millis(duration_ms),
        };
    }

    pub fn next(&mut self) {
        let now = Instant::now();
        let time_past = now.duration_since(self.lastcall);
        let time_left = match self.duration.checked_sub(time_past) {
            Some(time_left) => time_left,
            None => Duration::from_millis(0),
        };

        sleep(time_left);
        self.lastcall = Instant::now();
    }
}
