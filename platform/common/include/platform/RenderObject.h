#pragma once

namespace U {
	/// <summary>
	/// Wrapper class for platform specific renderable 'things' (usually a vbo/vao)
	/// <para/>
	/// Must be initialized with platform specific subclass
	/// </summary>
	class RenderObject {
	public:

		/// <summary>
		/// Binds this render object for the current render cycle.
		/// <para/>
		/// Usually involves setting or allocating buffers
		/// </summary>
		/// <returns>Count of vertices bound</returns>
		virtual long bind() = 0;

		/// <summary>
		/// Unbinds this render object for the current render cycle.
		/// </summary>
		virtual void unbind() = 0;

		virtual ~RenderObject() = default;

	protected:
		inline RenderObject() {};
	};
}