extends Node

class_name HealthComponenent

signal hp_changed(current_hp)
signal died()

@export var data: HealthResource

var current_hp: float

func _ready() -> void:
	current_hp = data.max_hp
func _take_damage(amount:float )->void:
	current_hp = max(current_hp-amount,0)
	emit_signal(&"hp_changed", current_hp)
	if current_hp == 0:
		emit_signal(&"died")
func _heal(amount:float)->void:
	current_hp= min(current_hp+amount, data.max_hp)
	emit_signal(&"hp_changed",current_hp)
