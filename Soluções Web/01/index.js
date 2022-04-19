function send() {
  const input = document.getElementById('chat-input');
  if (input.value.trim() === '') {
    alert('Digite alguma coisa para enviar');
    input.focus();
    return;
  }

  const chatBody = document.getElementById('chat-box-body-msgs');
  let userMessageBox = document.createElement('div'),
      userMessageBoxLabel = document.createElement('div'),
      userMessageBoxValue = document.createElement('div');

  userMessageBox.className = 'chat-box-body-user-msg';
  userMessageBoxLabel.className = 'chat-box-body-msg-label';
  userMessageBoxValue.className = 'chat-box-body-msg-value';

  userMessageBoxLabel.innerHTML = '<i>Você diz:</i>';
  userMessageBoxValue.innerHTML = input.value;

  userMessageBox.appendChild(userMessageBoxLabel);
  userMessageBox.appendChild(userMessageBoxValue);

  chatBody.appendChild(userMessageBox);
  input.value = '';
  input.focus();
}