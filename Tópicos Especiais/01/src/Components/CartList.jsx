import CartListItem from '../Components/CartListItem';

function CartList({ items, onUpdateItemQuantity, onRemoveItem }) {
  if (items.length <= 0) {
    return <p>Nothing in your cart yet.</p>;
  }

  return items.map((item) => (
    <CartListItem
      disabled={item.bought}
      title={item.title}
      description={item.description}
      price={item.price}
      image={item.image}
      quantity={item.quantity}
      key={item.id}
      onUpdateQuantity={(value) => {
        onUpdateItemQuantity(item, value);
      }}
      onRemoveItem={() => onRemoveItem(item)}
    />
  ));
}

export default CartList;
