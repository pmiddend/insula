class scene_manager
{
public:
	backend_ptr 
	create_backend(
		backend_parameters const &) const;

	instance_ptr
	create_instance(
		instance_parameters const &);

	instance_ptr
	create_transparent_instance(
		instance_parameters const &);

	void
	render();
};
