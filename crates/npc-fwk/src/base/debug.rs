#[macro_export]
macro_rules! dbg_out {
    ( $( $x:expr ),* ) => {
        print!("DEBUG: ");
        println!( $($x),* );
    };
}

/// Print an error message on the console.
#[macro_export]
macro_rules! err_out {
    ( $( $x:expr ),* ) => {
        {
            print!("ERROR: ");
            println!( $($x),* );
        }
    };
}

/// Like err_out!() but print the file and line number
#[macro_export]
macro_rules! err_out_line {
    ( $( $x:expr ),* ) => {
        {
            print!("ERROR: {}:{}:", file!(), line!());
            println!( $($x),* );
        }
    };
}

/// Assert and print a message if true.
/// Does NOT abort of call assert!()
#[macro_export]
macro_rules! dbg_assert {
    ( $cond:expr,  $msg:expr ) => {{
        if !$cond {
            print!("ASSERT: {}:{}: {}", file!(), line!(), stringify!($cond));
            println!($msg);
        }
    }};
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        dbg_out!("debug {}", 42);
        err_out!("error {}", 69);
        dbg_assert!(false, "failed assert");
    }
}
