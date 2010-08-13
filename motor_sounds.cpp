
using namespace insula;

int main(int, char *[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	fcppt::filesystem::path const 
		file_name = 
			media_path() / 
			FCPPT_TEXT("sounds") /
			FCPPT_TEXT("motor.wav");

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::parameterless::audio_player)
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))))
		(sge::systems::parameterless::input)
		(sge::window::parameters(
				FCPPT_TEXT("insula motor sounds"))
			.dim(
				sge::window::dim_type(640, 480)));

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				[&running]() { running = false; })));
	/*
	sge::audio::buffer_ptr const buf = 
		sys.audio_player()->create_buffer(
			sys.audio_loader().load(
				file_name));

	sge::audio::sound::base_ptr const s = 
		buf->create_nonpositional();

	s->play(
		sge::audio::sound::repeat::loop);
	*/

} catch (sge::audio::exception const &e) {
	fcppt::io::cerr << FCPPT_TEXT("audio exception caught: ") << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
} catch (sge::exception const &e) {
	fcppt::io::cerr << FCPPT_TEXT("Exception caught: ") << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
} catch (std::exception const &e) {
	std::cerr << "Exception caught: " << e.what() << '\n';
	return EXIT_FAILURE;
}
