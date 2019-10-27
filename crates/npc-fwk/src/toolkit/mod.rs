pub mod mimetype;

pub type Sender<T> = glib::Sender<T>;

/// Wrapper type for the channel tuple to get passed down to the unsafe C++ code.
pub struct PortableChannel<T>(pub Sender<T>, pub glib::SourceId);
