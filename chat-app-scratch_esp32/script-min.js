const socket = io('ws://10.0.0.8:8080')
const messageContainer = document.getElementById('message-container')
const messageForm = document.getElementById('send-container')
const messageInput = document.getElementById('message-input')

socket.on('chat-message', data => {
   appendMessage(data)
})

messageForm.addEventListener('submit', e => {
  e.preventDefault()
  const message = messageInput.value
  socket.emit('send-chat-message', message)
  messageInput.value = ''
})

function appendMessage(message) {
   const messageElement = document.createElement('div')
   messageElement.innerText = message
   messageContainer.append(messageElement)
}
