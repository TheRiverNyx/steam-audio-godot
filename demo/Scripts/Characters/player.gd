extends CharacterBody3D
class_name PlayerController

@onready var springarm:SpringArm3D = $Pivot/SpringArm3D

@export_category("Movement")
@export var speed:float = 14.0
@export var fall_acceleration:float = 75.0
@export var jump_speed:float = 10

@export_category("Camera")
@export var max_look_up:float = 80.0
@export var max_look_down:float = -80.0
@export var mouse_sensitivity:float= 0.15
@export var joystick_sensitivity:float = 100.00
var target_velocity:Vector3 = Vector3.ZERO

func _ready() -> void:
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	
func _physics_process(delta: float) -> void:
	var direction_2d = Input.get_vector(&"move_left",&"move_right",&"move_back",&"move_forward")
	var direction_3d = (transform.basis.x * direction_2d.x + -transform.basis.z * direction_2d.y).normalized()
	
	velocity.x = direction_3d.x*speed
	velocity.z = direction_3d.z*speed
	if not is_on_floor():
		velocity.y -= fall_acceleration * delta
	
	move_and_slide()
func _process(delta: float) -> void:
	var horiz = Input.get_axis(&"look_right",&"look_left")
	var vert = Input.get_axis(&"look_down",&"look_up")
	var camera_direction = Input.get_vector(&"look_right",&"look_left",&"look_down",&"look_up")
	if camera_direction != Vector2.ZERO:
		_apply_look(Vector2(horiz, vert)*(joystick_sensitivity*delta))

func _input(event: InputEvent) -> void:
	# mouse look (unchanged)
	if event is InputEventMouseMotion and Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		_apply_look(Vector2(-event.relative.x, -event.relative.y) * mouse_sensitivity)
	if event.is_action_pressed(&"jump") and is_on_floor():
		_jump()

func _apply_look(delta: Vector2) -> void:
	rotate_y(deg_to_rad(delta.x))
	
	var new_pitch = springarm.rotation_degrees.x + delta.y
	springarm.rotation_degrees.x = clamp(new_pitch,max_look_down,max_look_up)

func _jump() -> void:
	velocity.y = jump_speed
