
#[macro_export]
macro_rules! dbg_out {
    ( $( $x:expr ),* ) => {
        print!("DEBUG: ");
        println!( $($x),* );
    };
}

#[macro_export]
macro_rules! err_out {
    ( $( $x:expr ),* ) => {
        print!("ERROR: ");
        println!( $($x),* );
    };
}

#[macro_export]
macro_rules! dbg_assert {
    ( $cond:expr,  $msg:expr ) => {
        {
            if !$cond {
                print!("ASSERT: {}", stringify!($cond));
                println!( $msg );
            }
        }
    };
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        dbg_out!("debug {}", 42);
        err_out!("error {}", 69);
    }
}
