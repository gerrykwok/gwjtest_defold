--author: guoweiji
--date : 2019-03-12
--version : defold 1.2.148

crash = {}

--- <summary>
--- read backtrace recorded in a loaded crash dump
--- A table is returned containing the addresses of the call stack.
--- argument:
---      handle: (number) crash dump handle
--- return: backtrace: (table) table containing the backtrace
--- </summary>
function crash.get_backtrace(handle) end

--- <summary>
--- read text blob recorded in a crash dump
--- The format of read text blob is platform specific and not guaranteed but can be useful for manual inspection.
--- argument:
---      handle: (number) crash dump handle
--- return: blob: (string) string with the platform specific data
--- </summary>
function crash.get_extra_data(handle) end

--- <summary>
--- get all loaded modules from when the crash occured
--- The function returns a table containing entries with sub-tables that have fields 'name' and 'address' set for all loaded modules.
--- argument:
---      handle: (number) crash dump handle
--- return: modules: (table) module table
--- </summary>
function crash.get_modules(handle) end

--- <summary>
--- read signal number from a crash report
--- argument:
---      handle: (number) crash dump handle
--- return: signal:	(number) signal number
--- </summary>
function crash.get_signum(handle) end

--- <summary>
--- reads a system field from a loaded crash dump
--- argument:
---      handle: (number) crash dump handle
---      index:  (number) system field enum. Must be less than crash.SYSFIELD_MAX
--- return: value: (string) value recorded in the crash dump, or nil if it didn't exist
--- </summary>
function crash.get_sys_field(handle, index) end

--- <summary>
--- reads user field from a loaded crash dump
--- argument:
---      handle: (number) crash dump handle
---      index: (number) user data slot index
--- return: value: (string) user data value recorded in the crash dump
--- </summary>
function crash.get_user_field(handle, index) end

--- <summary>
--- loads a previously written crash dump
--- The crash dump will be removed from disk upon a successful load, so loading is one-shot.
--- return: handle: (number) handle to the loaded dump, or nil if no dump was found
--- </summary>
function crash.load_previous() end

--- <summary>
--- releases a previously loaded crash dump
--- argument:
---       handle: (number) handle to loaded crash dump
--- </summary>
function crash.release(handle) end

--- <summary>
--- sets the file location for crash dumps
--- Crashes occuring before the path is set will be stored to a default engine location.
--- argument:
---       path: (string) file path to use
--- </summary>
function crash.set_file_path(path) end

--- <summary>
--- stores user-defined string value
--- Store a user value that will get written to a crash dump when a crash occurs. This can be user id:s, breadcrumb data etc. There are 32 slots indexed from 0. Each slot stores at most 255 characters.
--- argument:
---        index: (number) slot index. 0-indexed
---        value: (string) string value to store
--- </summary>
function crash.set_user_field(index, value) end

--- <summary>
--- writes crash dump
--- Performs the same steps as if a crash had just occured but allows the program to continue. The generated dump can be read by crash.load_previous
--- </summary>
function crash.write_dump() end

gui = {}

--- <summary>
--- animates a node property
--- This starts an animation of a node property according to the specified parameters. If the node property is already being animated, that animation will be canceled and replaced by the new one. Note however that several different node properties can be animated simultaneously. Use gui.cancel_animation to stop the animation before it has completed.
---
--- Composite properties of type vector3, vector4 or quaternion also expose their sub-components (x, y, z and w). You can address the components individually by suffixing the name with a dot '.' and the name of the component. For instance, "position.x" (the position x coordinate) or "color.w" (the color alpha value).
--- 
--- If a complete_function (Lua function) is specified, that function will be called when the animation has completed. By starting a new animation in that function, several animations can be sequenced together. See the examples below for more information.
--- </summary>
function gui.animate(node, property, to, easing, duration, delay, complete_function, playback) end

--- <summary>
--- cancels an ongoing animation
--- If an animation of the specified node is currently running (started by gui.animate), it will immediately be canceled.
--- </summary>
function gui.cancel_animation(node, property) end

--- <summary>
--- cancel a node flipbook animation
--- Cancels any running flipbook animation on the specified node.
--- </summary>
function gui.cancel_flipbook(node) end

--- <summary>
--- cancel a spine animation
--- </summary>
function gui.cancel_spine(node) end

--- <summary>
--- clone a node
--- Make a clone instance of a node. This function does not clone the supplied node's children nodes. Use gui.clone_tree for that purpose.
--- </summary>
function gui.clone(node) end

--- <summary>
--- clone a node including its children
--- Make a clone instance of a node and all its children. Use gui.clone to clone a node excluding its children.
--- </summary>
function gui.clone_tree(node) end

--- <summary>
--- deletes a node
--- Deletes the specified node. Any child nodes of the specified node will be recursively deleted.
--- </summary>
function gui.delete_node(node) end

--- <summary>
--- delete texture
--- Delete a dynamically created texture.
--- </summary>
function gui.delete_texture(textureId) end

--- <summary>
--- gets the node adjust mode
--- Returns the adjust mode of a node. The adjust mode defines how the node will adjust itself to screen resolutions that differs from the one in the project settings.
--- </summary>
function gui.get_adjust_mode(node) end

--- <summary>
--- gets the node blend mode
--- Returns the blend mode of a node. Blend mode defines how the node will be blended with the background.
--- </summary>
function gui.get_blend_mode(node) end

--- <summary>
--- gets node clipping inverted state
--- If node is set as an inverted clipping node, it will clip anything inside as opposed to outside.
--- </summary>
function gui.get_clipping_inverted(node) end

--- <summary>
--- gets the node clipping mode
--- Clipping mode defines how the node will clipping it's children nodes
--- </summary>
function gui.get_clipping_mode(node) end

--- <summary>
--- gets node clipping visibility state
--- If node is set as visible clipping node, it will be shown as well as clipping. Otherwise, it will only clip but not show visually.
--- </summary>
function gui.get_clipping_visible(node) end

--- <summary>
--- gets the node color
--- Returns the color of the supplied node. The components of the returned vector4 contains the color channel values:
--- </summary>
function gui.get_color(node) end

--- <summary>
--- gets the angle for the filled pie sector
--- Returns the sector angle of a pie node.
--- </summary>
function gui.get_fill_angle(node) end

--- <summary>
--- gets the node flipbook animation
--- Get node flipbook animation.
--- </summary>
function gui.get_flipbook(node) end

--- <summary>
--- gets the node font
--- This is only useful for text nodes. The font must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.get_font(node) end

--- <summary>
--- gets the scene height
--- Returns the scene height.
--- </summary>
function gui.get_height() end

--- <summary>
--- gets the id of the specified node
--- Retrieves the id of the specified node.
--- </summary>
function gui.get_id(node) end

--- <summary>
--- gets the index of the specified node
--- Retrieve the index of the specified node. The index defines the order in which a node appear in a GUI scene. Higher index means the node is drawn on top of lower indexed nodes.
--- </summary>
function gui.get_index(node) end

--- <summary>
--- gets the node inherit alpha state
--- </summary>
function gui.get_inherit_alpha(node) end

--- <summary>
--- gets the pie inner radius
--- Returns the inner radius of a pie node. The radius is defined along the x-axis.
--- </summary>
function gui.get_inner_radius(node) end

--- <summary>
--- gets the node layer
--- The layer must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.get_layer(node) end

--- <summary>
--- gets the scene current layout
--- </summary>
function gui.get_layout() end

--- <summary>
--- gets the leading of the text node
--- Returns the leading value for a text node.
--- </summary>
function gui.get_leading(node) end

--- <summary>
--- get line-break mode
--- Returns whether a text node is in line-break mode or not. This is only useful for text nodes.
--- </summary>
function gui.get_line_break(node) end

--- <summary>
--- gets the node with the specified id
--- Retrieves the node with the specified id.
--- </summary>
function gui.get_node(id) end

--- <summary>
--- gets the pie outer bounds mode
--- Returns the outer bounds mode for a pie node.
--- </summary>
function gui.get_outer_bounds(node) end

--- <summary>
--- gets the node outline color
--- Returns the outline color of the supplied node. See gui.get_color for info how vectors encode color values.
--- </summary>
function gui.get_outline(node) end

--- <summary>
--- gets the parent of the specified node
--- Returns the parent node of the specified node. If the supplied node does not have a parent, nil is returned.
--- </summary>
function gui.get_parent(node) end

--- <summary>
--- Gets a particle fx
--- Get the paricle fx for a gui node
--- </summary>
function gui.get_particlefx(node) end

--- <summary>
--- gets the number of generated vertices around the perimeter
--- Returns the number of generated vertices around the perimeter of a pie node.
--- </summary>
function gui.get_perimeter_vertices(node) end

--- <summary>
--- gets the pivot of a node
--- The pivot specifies how the node is drawn and rotated from its position.
--- </summary>
function gui.get_pivot(node) end

--- <summary>
--- gets the node position
--- Returns the position of the supplied node.
--- </summary>
function gui.get_position(node) end

--- <summary>
--- gets the node rotation
--- Returns the rotation of the supplied node. The rotation is expressed in degree Euler angles.
--- </summary>
function gui.get_rotation(node) end

--- <summary>
--- gets the node scale
--- Returns the scale of the supplied node.
--- </summary>
function gui.get_scale(node) end

--- <summary>
--- gets the node screen position
--- Returns the screen position of the supplied node. This function returns the calculated transformed position of the node, taking into account any parent node transforms.
--- </summary>
function gui.get_screen_position(node) end

--- <summary>
--- gets the node shadow color
--- Returns the shadow color of the supplied node. See gui.get_color for info how vectors encode color values.
--- </summary>
function gui.get_shadow(node) end

--- <summary>
--- gets the node size
--- Returns the size of the supplied node.
--- </summary>
function gui.get_size(node) end

--- <summary>
--- gets the node size mode
--- Returns the size of a node. The size mode defines how the node will adjust itself in size. Automatic size mode alters the node size based on the node's content.
--- </summary>
function gui.get_size_mode(node) end

--- <summary>
--- get the slice9 values for the node
--- Returns the slice9 configuration values for the node.
--- </summary>
function gui.get_slice9(node) end

--- <summary>
--- gets the playing animation on a spine node
--- Gets the playing animation on a spine node
--- </summary>
function gui.get_spine_animation(node) end

--- <summary>
--- retrieve the GUI node corresponding to a spine skeleton bone
--- The returned node can be used for parenting and transform queries. This function has complexity O(n), where n is the number of bones in the spine model skeleton.
--- </summary>
function gui.get_spine_bone(node, bone_id) end

--- <summary>
--- gets the normalized cursor of the animation on a spine node
--- This is only useful for spine nodes. Gets the normalized cursor of the animation on a spine node.
--- </summary>
function gui.get_spine_cursor(node) end

--- <summary>
--- gets the playback rate of the animation on a spine node
--- This is only useful for spine nodes. Gets the playback rate of the animation on a spine node.
--- </summary>
function gui.get_spine_playback_rate(node) end

--- <summary>
--- gets the spine scene of a node
--- Returns the spine scene id of the supplied node. This is currently only useful for spine nodes. The returned spine scene must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.get_spine_scene(node) end

--- <summary>
--- gets the skin of a spine node
--- Gets the spine skin of a spine node
--- </summary>
function gui.get_spine_skin(node) end

--- <summary>
--- gets the node text
--- Returns the text value of a text node. This is only useful for text nodes.
--- </summary>
function gui.get_text(node) end

--- <summary>
--- get text metrics
--- Get text metrics given the provided font, text and parameters.
--- </summary>
function gui.get_text_metrics(font, text, width, line_break, leading, tracking) end

--- <summary>
--- get text metrics from node
--- Get the text metrics from a text node.
--- </summary>
function gui.get_text_metrics_from_node(node) end

--- <summary>
--- gets node texture
--- Returns the texture of a node. This is currently only useful for box or pie nodes. The texture must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.get_texture(node) end

--- <summary>
--- gets the tracking of the text node
--- Returns the tracking value of a text node.
--- </summary>
function gui.get_tracking(node) end

--- <summary>
--- gets the scene width
--- Returns the scene width.
--- </summary>
function gui.get_width() end

--- <summary>
--- gets the x-anchor of a node
--- The x-anchor specifies how the node is moved when the game is run in a different resolution.
--- </summary>
function gui.get_xanchor(node) end

--- <summary>
--- gets the y-anchor of a node
--- The y-anchor specifies how the node is moved when the game is run in a different resolution.
--- </summary>
function gui.get_yanchor(node) end

--- <summary>
--- hides on-display keyboard if available
--- Hides the on-display touch keyboard on the device.
--- </summary>
function gui.hide_keyboard() end

--- <summary>
--- returns if a node is enabled or not
--- Returns true if a node is enabled and false if it's not. Disabled nodes are not rendered and animations acting on them are not evaluated.
--- </summary>
function gui.is_enabled(node) end

--- <summary>
--- moves the first node above the second
--- Alters the ordering of the two supplied nodes by moving the first node above the second. If the second argument is nil the first node is moved to the top.
--- </summary>
function gui.move_above(node, node) end

--- <summary>
--- moves the first node below the second
--- Alters the ordering of the two supplied nodes by moving the first node below the second. If the second argument is nil the first node is moved to the bottom.
--- </summary>
function gui.move_below(node, node) end

--- <summary>
--- creates a new box node
--- Dynamically create a new box node.
--- </summary>
function gui.new_box_node(pos, size) end

--- <summary>
--- creates a new particle fx node
--- Dynamically create a particle fx node.
--- </summary>
function gui.new_particlefx_node(pos, particlefx) end

--- <summary>
--- creates a new pie node
--- Dynamically create a new pie node.
--- </summary>
function gui.new_pie_node(pos, size) end

--- <summary>
--- creates a new spine node
--- Dynamically create a new spine node.
--- </summary>
function gui.new_spine_node(pos, spine_scene) end

--- <summary>
--- creates a new text node
--- Dynamically create a new text node.
--- </summary>
function gui.new_text_node(pos, text) end

--- <summary>
--- create new texture
--- Dynamically create a new texture.
--- </summary>
function gui.new_texture(textureId, width, height, type, buffer, flip) end

--- <summary>
--- determines if the node is pickable by the supplied coordinates
--- Tests whether a coordinate is within the bounding box of a node.
--- </summary>
function gui.pick_node(node, x, y) end

--- <summary>
--- play node flipbook animation
--- Play flipbook animation on a box or pie node. The current node texture must contain the animation. Use this function to set one-frame still images on the node.
--- </summary>
function gui.play_flipbook(node, animation, complete_function) end

--- <summary>
--- Plays a particle fx
--- Plays the paricle fx for a gui node
--- </summary>
function gui.play_particlefx(node, emitter_state_function) end

--- <summary>
--- play a spine animation
--- Starts a spine animation.
--- </summary>
function gui.play_spine_anim(node, animation_id, playback, play_properties, complete_function) end

--- <summary>
--- resets on-display keyboard if available
--- Resets the input context of keyboard. This will clear marked text.
--- </summary>
function gui.reset_keyboard() end

--- <summary>
--- resets all nodes to initial state
--- Resets all nodes in the current GUI scene to their initial state. The reset only applies to static node loaded from the scene. Nodes that are created dynamically from script are not affected.
--- </summary>
function gui.reset_nodes() end

--- <summary>
--- sets node adjust mode
--- Sets the adjust mode on a node. The adjust mode defines how the node will adjust itself to screen resolutions that differs from the one in the project settings.
--- </summary>
function gui.set_adjust_mode(node, adjust_mode) end

--- <summary>
--- sets node blend mode
--- Set the blend mode of a node. Blend mode defines how the node will be blended with the background.
--- </summary>
function gui.set_blend_mode(node, blend_mode) end

--- <summary>
--- sets node clipping inversion
--- If node is set as an inverted clipping node, it will clip anything inside as opposed to outside.
--- </summary>
function gui.set_clipping_inverted(node, inverted) end

--- <summary>
--- sets node clipping mode state
--- Clipping mode defines how the node will clipping it's children nodes
--- </summary>
function gui.set_clipping_mode(node, clipping_mode) end

--- <summary>
--- sets node clipping visibility
--- If node is set as an visible clipping node, it will be shown as well as clipping. Otherwise, it will only clip but not show visually.
--- </summary>
function gui.set_clipping_visible(node, visible) end

--- <summary>
--- sets the node color
--- Sets the color of the supplied node. The components of the supplied vector3 or vector4 should contain the color channel values:
--- </summary>
function gui.set_color(node, color) end

--- <summary>
--- enables/disables a node
--- Sets a node to the disabled or enabled state. Disabled nodes are not rendered and animations acting on them are not evaluated.
--- </summary>
function gui.set_enabled(node, enabled) end

--- <summary>
--- sets the angle for the filled pie sector
--- Set the sector angle of a pie node.
--- </summary>
function gui.set_fill_angle(node, angle) end

--- <summary>
--- sets the node font
--- This is only useful for text nodes. The font must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.set_font(node, fontId) end

--- <summary>
--- sets the id of the specified node
--- Set the id of the specicied node to a new value. Nodes created with the gui.new_*_node() functions get an empty id. This function allows you to give dynamically created nodes an id.
---
--- No checking is done on the uniqueness of supplied ids. It is up to you to make sure you use unique ids.
--- </summary>
function gui.set_id(node, id) end

--- <summary>
--- sets the node inherit alpha state
--- </summary>
function gui.set_inherit_alpha(node, inherit_alpha) end

--- <summary>
--- sets the pie inner radius
--- Sets the inner radius of a pie node. The radius is defined along the x-axis.
--- </summary>
function gui.set_inner_radius(node, radius) end

--- <summary>
--- sets the node layer
--- The layer must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.set_layer(node, layer) end

--- <summary>
--- sets the leading of the text node
--- Sets the leading value for a text node. This value is used to scale the line spacing of text.
--- </summary>
function gui.set_leading(node, leading) end

--- <summary>
--- set line-break mode
--- Sets the line-break mode on a text node. This is only useful for text nodes.
--- </summary>
function gui.set_line_break(node, line_break) end

--- <summary>
--- sets the pie node outer bounds mode
--- Sets the outer bounds mode for a pie node.
--- </summary>
function gui.set_outer_bounds(node, bounds_mode) end

--- <summary>
--- sets the node outline color
--- Sets the outline color of the supplied node. See gui.set_color for info how vectors encode color values.
--- </summary>
function gui.set_outline(node, color) end

--- <summary>
--- sets the parent of the node
--- Sets the parent node of the specified node.
--- </summary>
function gui.set_parent(node, parent, keep_scene_transform) end

--- <summary>
--- Sets a particle fx
--- Set the paricle fx for a gui node
--- </summary>
function gui.set_particlefx(node, particlefx) end

--- <summary>
--- sets the number of generated vertices around the perimeter
--- Sets the number of generated vertices around the perimeter of a pie node.
--- </summary>
function gui.set_perimeter_vertices(node, vertices) end

--- <summary>
--- sets the pivot of a node
--- The pivot specifies how the node is drawn and rotated from its position.
--- </summary>
function gui.set_pivot(node, pivot) end

--- <summary>
--- sets the node position
--- Sets the position of the supplied node.
--- </summary>
function gui.set_position(node, position) end

--- <summary>
--- sets the render ordering for the current GUI scene
--- Set the order number for the current GUI scene. The number dictates the sorting of the "gui" render predicate, in other words in which order the scene will be rendered in relation to other currently rendered GUI scenes.
--- </summary>
function gui.set_render_order(order) end

--- <summary>
--- sets the node rotation
--- Sets the rotation of the supplied node. The rotation is expressed in degree Euler angles.
--- </summary>
function gui.set_rotation(node, rotation) end

--- <summary>
--- sets the node scale
--- Sets the scaling of the supplied node.
--- </summary>
function gui.set_scale(node, scale) end

--- <summary>
--- sets the node shadow color
--- Sets the shadow color of the supplied node. See gui.set_color for info how vectors encode color values.
--- </summary>
function gui.set_shadow(node, color) end

--- <summary>
--- sets the node size
--- Sets the size of the supplied node.
---
--- You can only set size on nodes with size mode set to SIZE_MODE_MANUAL
--- </summary>
function gui.set_size(node, size) end

--- <summary>
--- sets node size mode
--- ets the size mode of a node. The size mode defines how the node will adjust itself in size. Automatic size mode alters the node size based on the node's content.
--- </summary>
function gui.set_size_mode(node, size_mode) end

--- <summary>
--- set the slice9 configuration for the node
--- Set the slice9 configuration values for the node.(vector4)
--- </summary>
function gui.set_slice9(node, values) end

--- <summary>
--- sets the normalized cursor of the animation on a spine node
--- This is only useful for spine nodes. The cursor is normalized.
--- </summary>
function gui.set_spine_cursor(node, cursor) end

--- <summary>
--- sets the playback rate of the animation on a spine node
--- This is only useful for spine nodes. Sets the playback rate of the animation on a spine node. Must be positive.
--- </summary>
function gui.set_spine_playback_rate(node, playback_rate) end

--- <summary>
--- sets the spine scene of a node
--- Set the spine scene on a spine node. The spine scene must be mapped to the gui scene in the gui editor.
--- </summary>
function gui.set_spine_scene(node, spine_scene) end

--- <summary>
--- sets the spine skin
--- Sets the spine skin on a spine node.
--- </summary>
function gui.set_spine_skin(node, spine_skin, spine_slot) end

--- <summary>
--- sets the node text
--- Set the text value of a text node. This is only useful for text nodes.
--- </summary>
function gui.set_text(node, text) end

--- <summary>
--- sets the node texture
--- Set the texture on a box or pie node. The texture must be mapped to the gui scene in the gui editor. The function points out which texture the node should render from. If the texture is an atlas, further information is needed to select which image/animation in the atlas to render. In such cases, use gui.play_flipbook() in addition to this function.
--- </summary>
function gui.set_texture(node, textureId) end

--- <summary>
--- set the buffer data for a texture
--- Set the texture buffer data for a dynamically created texture.
--- </summary>
function gui.set_texture_data(texture, width, height, texture_type, buffer, flip) end

--- <summary>
--- sets the tracking of the text node
--- Sets the tracking value of a text node. This value is used to adjust the vertical spacing of characters in the text.
--- </summary>
function gui.set_tracking(node, tracking) end

--- <summary>
--- sets the x-anchor of a node
--- The x-anchor specifies how the node is moved when the game is run in a different resolution.
--- </summary>
function gui.set_xanchor(node, anchor) end

--- <summary>
--- sets the y-anchor of a node
--- The y-anchor specifies how the node is moved when the game is run in a different resolution.
--- </summary>
function gui.set_yanchor(node, anchor) end

--- <summary>
--- shows the on-display keyboard if available
--- Shows the on-display touch keyboard. The specified type of keyboard is displayed if it is available on the device.
---
--- This function is only available on iOS and Android. 
--- </summary>
function gui.show_keyboard(keyboard_type, autoclose) end

--- <summary>
--- Stops a particle fx
--- Stops the particle fx for a gui node
--- </summary>
function gui.stop_particlefx(node) end

--- <summary>
--- fit adjust mode
--- </summary>
gui.ADJUST_FIT = 1
--- <summary>
--- stretch adjust mode
--- </summary>
gui.ADJUST_STRETCH = 1
--- <summary>
--- zoom adjust mode
--- </summary>
gui.ADJUST_ZOOM = 1
--- <summary>
--- bottom y-anchor
--- </summary>
gui.ANCHOR_BOTTOM = 1
--- <summary>
--- left x-anchor
--- </summary>
gui.ANCHOR_LEFT = 1
--- <summary>
--- right x-anchor
--- </summary>
gui.ANCHOR_RIGHT = 1
--- <summary>
--- top y-anchor
--- </summary>
gui.ANCHOR_TOP = 1
--- <summary>
--- additive blending
--- </summary>
gui.BLEND_ADD = 1
--- <summary>
--- additive alpha blending
--- </summary>
gui.BLEND_ADD_ALPHA = 1
--- <summary>
--- alpha blending
--- </summary>
gui.BLEND_ALPHA = 1
--- <summary>
--- multiply blending
--- </summary>
gui.BLEND_MULT = 1
--- <summary>
--- clipping mode none
--- </summary>
gui.CLIPPING_MODE_NONE = 1
--- <summary>
--- clipping mode stencil
--- </summary>
gui.CLIPPING_MODE_STENCIL = 1
--- <summary>
--- in-back
--- </summary>
gui.EASING_INBACK = 1
--- <summary>
--- in-bounce
--- </summary>
gui.EASING_INBOUNCE = 1
--- <summary>
--- in-circlic
--- </summary>
gui.EASING_INCIRC = 1
--- <summary>
--- in-cubic
--- </summary>
gui.EASING_INCUBIC = 1
--- <summary>
--- in-elastic
--- </summary>
gui.EASING_INELASTIC = 1
--- <summary>
--- in-exponential
--- </summary>
gui.EASING_INEXPO = 1
--- <summary>
--- in-out-back
--- </summary>
gui.EASING_INOUTBACK = 1
--- <summary>
--- in-out-bounce
--- </summary>
gui.EASING_INOUTBOUNCE = 1
--- <summary>
--- in-out-circlic
--- </summary>
gui.EASING_INOUTCIRC = 1
--- <summary>
--- in-out-cubic
--- </summary>
gui.EASING_INOUTCUBIC = 1
--- <summary>
--- in-out-elastic
--- </summary>
gui.EASING_INOUTELASTIC = 1
--- <summary>
--- in-out-exponential
--- </summary>
gui.EASING_INOUTEXPO = 1
--- <summary>
--- in-out-quadratic
--- </summary>
gui.EASING_INOUTQUAD = 1
--- <summary>
--- in-out-quartic
--- </summary>
gui.EASING_INOUTQUART = 1
--- <summary>
--- in-out-quintic
--- </summary>
gui.EASING_INOUTQUINT = 1
--- <summary>
--- in-out-sine
--- </summary>
gui.EASING_INOUTSINE = 1
--- <summary>
--- in-quadratic
--- </summary>
gui.EASING_INQUAD = 1
--- <summary>
--- in-quartic
--- </summary>
gui.EASING_INQUART = 1
--- <summary>
--- in-quintic
--- </summary>
gui.EASING_INQUINT = 1
--- <summary>
--- in-sine
--- </summary>
gui.EASING_INSINE = 1
--- <summary>
--- linear interpolation
--- </summary>
gui.EASING_LINEAR = 1
--- <summary>
--- out-back
--- </summary>
gui.EASING_OUTBACK = 1
--- <summary>
--- out-bounce
--- </summary>
gui.EASING_OUTBOUNCE = 1
--- <summary>
--- out-circlic
--- </summary>
gui.EASING_OUTCIRC = 1
--- <summary>
--- out-cubic
--- </summary>
gui.EASING_OUTCUBIC = 1
--- <summary>
--- out-elastic
--- </summary>
gui.EASING_OUTELASTIC = 1
--- <summary>
--- out-exponential
--- </summary>
gui.EASING_OUTEXPO = 1
--- <summary>
--- out-in-back
--- </summary>
gui.EASING_OUTINBACK = 1
--- <summary>
--- out-in-bounce
--- </summary>
gui.EASING_OUTINBOUNCE = 1
--- <summary>
--- out-in-circlic
--- </summary>
gui.EASING_OUTINCIRC = 1
--- <summary>
--- out-in-cubic
--- </summary>
gui.EASING_OUTINCUBIC = 1
--- <summary>
--- out-in-elastic
--- </summary>
gui.EASING_OUTINELASTIC = 1
--- <summary>
--- out-in-exponential
--- </summary>
gui.EASING_OUTINEXPO = 1
--- <summary>
--- out-in-quadratic
--- </summary>
gui.EASING_OUTINQUAD = 1
--- <summary>
--- out-in-quartic
--- </summary>
gui.EASING_OUTINQUART = 1
--- <summary>
--- out-in-quintic
--- </summary>
gui.EASING_OUTINQUINT = 1
--- <summary>
--- out-in-sine
--- </summary>
gui.EASING_OUTINSINE = 1
--- <summary>
--- out-quadratic
--- </summary>
gui.EASING_OUTQUAD = 1
--- <summary>
--- out-quartic
--- </summary>
gui.EASING_OUTQUART = 1
--- <summary>
--- out-quintic
--- </summary>
gui.EASING_OUTQUINT = 1
--- <summary>
--- out-sine
--- </summary>
gui.EASING_OUTSINE = 1
--- <summary>
--- default keyboard
--- </summary>
gui.KEYBOARD_TYPE_DEFAULT = 1
--- <summary>
--- email keyboard
--- </summary>
gui.KEYBOARD_TYPE_EMAIL = 1
--- <summary>
--- number input keyboard
--- </summary>
gui.KEYBOARD_TYPE_NUMBER_PAD = 1
--- <summary>
--- password keyboard
--- </summary>
gui.KEYBOARD_TYPE_PASSWORD = 1
--- <summary>
--- elliptical pie node bounds
--- </summary>
gui.PIEBOUNDS_ELLIPSE = 1
--- <summary>
--- rectangular pie node bounds
--- </summary>
gui.PIEBOUNDS_RECTANGLE = 1
--- <summary>
--- center pivor
--- </summary>
gui.PIVOT_CENTER = 1
--- <summary>
--- east pivot
--- </summary>
gui.PIVOT_E = 1
--- <summary>
--- north pivot
--- </summary>
gui.PIVOT_N = 1
--- <summary>
--- north-east pivot
--- </summary>
gui.PIVOT_NE = 1
--- <summary>
--- north-west pivot
--- </summary>
gui.PIVOT_NW = 1
--- <summary>
--- south pivot
--- </summary>
gui.PIVOT_S = 1
--- <summary>
--- south-east pivot
--- </summary>
gui.PIVOT_SE = 1
--- <summary>
--- south-west pivot
--- </summary>
gui.PIVOT_SW = 1
--- <summary>
--- west pivot
--- </summary>
gui.PIVOT_W = 1
--- <summary>
--- loop backward
--- </summary>
gui.PLAYBACK_LOOP_BACKWARD = 1
--- <summary>
--- loop forward
--- </summary>
gui.PLAYBACK_LOOP_FORWARD = 1
--- <summary>
--- ping pong loop
--- </summary>
gui.PLAYBACK_LOOP_PINGPONG = 1
--- <summary>
--- once backward
--- </summary>
gui.PLAYBACK_ONCE_BACKWARD = 1
--- <summary>
--- once forward
--- </summary>
gui.PLAYBACK_ONCE_FORWARD = 1
--- <summary>
--- once forward and then backward
--- </summary>
gui.PLAYBACK_ONCE_PINGPONG = 1
--- <summary>
--- color property
--- </summary>
gui.PROP_COLOR = 1
--- <summary>
--- fill_angle property
--- </summary>
gui.PROP_FILL_ANGLE = 1
--- <summary>
--- inner_radius property
--- </summary>
gui.PROP_INNER_RADIUS = 1
--- <summary>
--- outline color property
--- </summary>
gui.PROP_OUTLINE = 1
--- <summary>
--- position property
--- </summary>
gui.PROP_POSITION = 1
--- <summary>
--- rotation property
--- </summary>
gui.PROP_ROTATION = 1
--- <summary>
--- scale property
--- </summary>
gui.PROP_SCALE = 1
--- <summary>
--- shadow color property
--- </summary>
gui.PROP_SHADOW = 1
--- <summary>
--- size property
--- </summary>
gui.PROP_SIZE = 1
--- <summary>
--- slice9 property
--- </summary>
gui.PROP_SLICE9 = 1
--- <summary>
--- automatic size mode
--- </summary>
gui.SIZE_MODE_AUTO = 1
--- <summary>
--- manual size mode
--- </summary>
gui.SIZE_MODE_MANUAL = 1

sys = {}

--- <summary>
--- get application information
--- Returns a table with application information for the requested app.
--- 
--- On iOS, the app_string is an url scheme for the app that is queried. Your game needs to list the schemes that are queried in an LSApplicationQueriesSchemes array in a custom "Info.plist".
--- 
--- On Android, the app_string is the package identifier for the app.
--- </summary>
function sys.get_application_info(app_string) end

--- <summary>
--- get config value
--- Get config value from the game.project configuration file.
--- 
--- In addition to the project file, configuration values can also be passed to the runtime as command line arguments with the --config argument.
--- </summary>
function sys.get_config(key) end

--- <summary>
--- get config value with default value
--- Get config value from the game.project configuration file with default value
--- </summary>
function sys.get_config(key, default_value) end

--- <summary>
--- get current network connectivity status
--- Returns the current network connectivity status on mobile platforms.
--- 
--- On desktop, this function always return sys.NETWORK_CONNECTED.
--- </summary>
function sys.get_connectivity() end

--- <summary>
--- get engine information
--- Returns a table with engine information.
--- table with engine information in the following fields:
---      version (string)
---          The current Defold engine version, i.e. "1.2.96"
---      version_sha1 (string)
---          The SHA1 for the current engine build, i.e. "0060183cce2e29dbd09c85ece83cbb72068ee050"
---      is_debug (boolean)
---          If the engine is a debug or release version
--- </summary>
function sys.get_engine_info() end

--- <summary>
--- enumerate network interfaces
--- Returns an array of tables with information on network interfaces.
--- an array of tables. Each table entry contain the following fields:
---      name
---          string Interface name
---      address
---          string IP address. might be nil if not available.
---      mac
---          string Hardware MAC address. might be nil if not available.
---      up
---          boolean true if the interface is up (available to transmit and receive data), false otherwise.
---      running
---          boolean true if the interface is running, false otherwise. 
--- </summary>
function sys.get_ifaddrs() end

--- <summary>
--- gets the save-file path
--- The save-file path is operating system specific and is typically located under the user's home directory.
--- </summary>
function sys.get_save_file(application_id, file_name) end

--- <summary>
--- get system information
--- Returns a table with system information.
--- table with system information in the following fields:
---      device_model
---          string Only available on iOS and Android.
---      manufacturer
---          string Only available on iOS and Android.
---      system_name
---          string The system OS name: "Darwin", "Linux", "Windows", "HTML5", "Android" or "iPhone OS"
---      system_version
---          string The system OS version.
---      api_version
---          string The API version on the system.
---      language
---          string Two character ISO-639 format, i.e. "en".
---      device_language
---          string Two character ISO-639 format (i.e. "sr") and, if applicable, followed by a dash (-) and an ISO 15924 script code (i.e. "sr-Cyrl" or "sr-Latn"). Reflects the device preferred language.
---      territory
---          string Two character ISO-3166 format, i.e. "US".
---      gmt_offset
---          number The current offset from GMT (Greenwich Mean Time), in minutes.
---      device_ident
---          string "identifierForVendor" on iOS. "android_id" on Android.
---      ad_ident
---          string "advertisingIdentifier" on iOS. advertising ID provided by Google Play on Android.
---      ad_tracking_enabled
---          boolean true if ad tracking is enabled, false otherwise.
---      user_agent
---          string The HTTP user agent, i.e. "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8" 
--- </summary>
function sys.get_sys_info() end

--- <summary>
--- loads a lua table from a file on disk
--- If the file exists, it must have been created by sys.save to be loaded.
--- </summary>
function sys.load(filename) end

--- <summary>
--- loads resource from game data
--- Loads a custom resource. Specify the full filename of the resource that you want to load. When loaded, the file data is returned as a string. If loading fails, the function returns nil.
--- 
--- In order for the engine to include custom resources in the build process, you need to specify them in the "custom_resources" key in your "game.project" settings file. You can specify single resource files or directories. If a directory is included in the resource list, all files and directories in that directory is recursively included:
--- 
--- For example "main/data/,assets/level_data.json".
--- </summary>
function sys.load_resource(filename) end

--- <summary>
--- open url in default application
--- Open URL in default application, typically a browser
--- </summary>
function sys.open_url(url) end

--- <summary>
--- saves a lua table to a file stored on disk
--- The table can later be loaded by sys.load. Use sys.get_save_file to obtain a valid location for the file. Internally, this function uses a workspace buffer sized output file sized 512kb. This size reflects the output file size which must not exceed this limit. Additionally, the total number of rows that any one table may contain is limited to 65536 (i.e. a 16 bit range). When tables are used to represent arrays, the values of keys are permitted to fall within a 32 bit range, supporting sparse arrays, however the limit on the total number of rows remains in effect.
--- </summary>
function sys.save(filename, t) end

--- <summary>
--- set host to check for network connectivity against
--- Sets the host that is used to check for network connectivity against.
--- </summary>
function sys.set_connectivity_host(host) end

--- <summary>
--- set the error handler
--- Set the Lua error handler function. The error handler is a function which is called whenever a lua runtime error occurs.
--- </summary>
function sys.set_error_handler(error_handler) end

--- <summary>
--- network connected through other, non cellular, connection
--- </summary>
sys.NETWORK_CONNECTED = 0

--- <summary>
--- network connected through mobile cellular
--- </summary>
sys.NETWORK_CONNECTED_CELLULAR = 0

--- <summary>
--- no network connection found
--- </summary>
sys.NETWORK_DISCONNECTED = 0
